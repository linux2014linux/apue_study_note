为何不会产生死循环,原因如下:

1.linux中信号处理程序中使用longjmp跳出时,不会恢复信号屏蔽字.
2.信号处理程序执行时,会将当前信号添加到信号屏蔽字中.

因此当第一次alarm触发后,执行sig_alarm时SIGALRM被加入信号屏蔽字中,之后使用longjmp跳出,此时SIGALARM并未从屏蔽字中跳出,因此该信号后续一直被阻塞.所以就无法产生死循环了.
