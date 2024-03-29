#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <lualib.h>
#include <lauxlib.h>

// /* thread status */
// #define LUA_OK	  0
// #define LUA_YIELD	  1
// #define LUA_ERRRUN  2
// #define LUA_ERRSYNTAX	  3
// #define LUA_ERRMEM  4
// #define LUA_ERRERR  5
static const char *resume_msg[] = {
	"LUA_OK",
	"LUA_YIELD",
	"LUA_ERRRUN",
	"LUA_ERRSYNTAX",
	"LUA_ERRMEM",
	"LUA_ERRERR",
};

static lua_State *gL;

struct sleep_data
{
	int timeout;
	lua_State *th;
};
static struct sleep_data sleep_data[10];

static int my_sleep(lua_State *L)
{
	int n = lua_tonumber(L, -1);
	lua_pop(L, 1);

	for (int i = 0; i < 10; ++i)
	{
		if (sleep_data[i].timeout != -1)
			continue;
		sleep_data[i].timeout = n;
		sleep_data[i].th = L;
		return lua_yield(L, 0);
	}
	return 0;
}

static int my_th2(lua_State *L)
{
	return 0;
}

static void lua_init()
{
	gL = luaL_newstate();
	luaL_openlibs(gL);

    lua_createtable(gL, 0 /* narr */, 1 /* nrec */);

	lua_pushcfunction(gL, my_sleep);
	lua_setfield(gL, -2, "sleep");

	lua_pushcfunction(gL, my_th2);
	lua_setfield(gL, -2, "th2");

	lua_setglobal(gL, "ngx");

	
//	lua_pushcfunction(L, my_sleep);
//	lua_setfield(L, -2, "sleep");
//	lua_setglobal(L, "ngx");
}

static void run_thread(const char *func_name)
{
	lua_State *th1 = lua_newthread(gL);
	lua_getglobal(th1, func_name);
	int ret = lua_resume(th1, 0);
	printf("run_thread %s, return %s\n", func_name, resume_msg[ret]);
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < 10; ++i) {
		sleep_data[i].timeout = -1;
		sleep_data[i].th = NULL;		
	}

    lua_init();
	luaL_dofile(gL, "th1.lua");

	run_thread("th1_func");
	run_thread("th2_func");
	run_thread("th3_func");
	run_thread("th4_func");	
	
	for (;;) {
		sleep(1);
		printf("sleep 1 sec\n");
		for (int i = 0; i < 10; ++i)
		{
			if (sleep_data[i].timeout == -1)
				continue;
			assert(sleep_data[i].th != NULL);
			--sleep_data[i].timeout;
			if (sleep_data[i].timeout == 0)
			{
				sleep_data[i].timeout = -1;
				int ret = lua_resume(sleep_data[i].th, 0);
				printf("sleep_data %d, return %s\n", i, resume_msg[ret]);
			}
		}
	}
    return 0;
}
