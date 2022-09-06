function th1_func()
    print("enter th1")
    print("th1: ready to sleep 5 sec")
    ngx.sleep(5)
    print("th1: resume from sleep")
    print("leave th1")
end

function th2_func()
    print("enter th2")
    print("th2: ready to sleep 3 sec")
    ngx.sleep(3)
    print("th2: resume from sleep")
    print("leave th2")
end

function th3_func()
    print("enter th3")
    print("th3 do nothing")    
    print("leave th3")
end

function th4_func()
    print("enter th4")
    print("th4: ready to throw err")
    aaa.bbb = ccc.ddd
    print("th4: after throw err")
    print("leave th4")
end
