function th1_func()
    print("enter th1")
    print("th1: ready to sleep")
    ngx.sleep(5)
    print("th1: resume from sleep")
    print("leave th1")
end

function th2_func()
    print("enter th2")
    print("th2: ready to sleep")
    ngx.sleep(3)
    print("th2: resume from sleep")
    print("leave th2")
end

