#read_timeout_by_signal.cpp
read属于自动重启的系统调用,因此当sig_alrm被执行时如果仅仅是return那么在sig_alrm执行完成后,read将继续等待,此时超时完全没有意义.而如果在sig_alrm中使用exit(0)可中断read操作,但这将终止整个进程,exit操作毫无意义.
另外注意,在alarm(10)和read之间如果发生阻塞并且超过10,那么read可能永远阻塞.可在中间加sleep来模拟测试.


#read_timeout_by_sigaction.cpp
参考:https://www.cnblogs.com/black-mamba/p/6870119.html
使用signal和sigaction对比,可重启和不可重启两种信号模式下的系统调用.
sigaction的对于信号的处理灵活很多,比signal略显复杂.
