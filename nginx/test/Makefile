all: lua_thread
CFLAGS = -g -O0 -I/usr/include/lua5.1
CC = gcc

lua_thread: lua_thread.c
	${CC} ${CFLAGS} -llua5.1 -o $@ $^ 

clean:
	rm -f lua_thread
