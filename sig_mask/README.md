注意,信号屏蔽不是忽略

sigprocmask()的作用，主要就是屏蔽指定的信号。这个 “屏蔽” 的含义需要说明清楚。首先我们大致数一下信号在内核里的处理流程吧（不是准确的流程，只是便于说明）：
1.内核等待信号中断
2.信号产生，触发内核中断
3.内核将信号存下来，或者说设置信号标志
4.内核根据用户空间的配置，处理信号。如果用户空间没有特别配置，则按照默认行为处理
5.处理完成后，清除信号标志
6.回到 1，继续等待
sigprocmask()所做的 “屏蔽”，其实就是将上述的信号处理流程，卡在了 3 和 4 之间，让内核能够将信号标志设置好，但是却到不了判断并处理的那一步。
换句话说，即便进程调用 signal() 函数，设置了 SIG_IGN 标志，但如果指定的信号被 sigprocmask() 屏蔽了的话，内核也不会去判断是否该忽略这个信号，而只是把信号标志卡在那儿，直到调用sigprocmask()执行SIG_UNBLOCK为止，才能让内核继续走到第 4 步。

参考:https://segmentfault.com/a/1190000009129036
