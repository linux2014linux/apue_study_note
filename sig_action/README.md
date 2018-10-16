代码说明
sigaction_mask.cpp
SIGUSR1处理期间阻塞SIGUSR2,SIGSUR2信号无法打断SIGUSR1,SIGUSR2会阻塞到SIGUSR1执行完再执行.
注意,如果在使用sigaction之后又使用signal,那么是会被覆盖的.

sigaction_nomask_nosyscall.cpp
SIGUSR1不阻塞SIGUSR2.SIGUSR1在信号处理程序中不使用系统调用,使用普通循环,当SIGUSR2在SIGUSR1处理过程中发生,SIGUSR2打断SIGUSR1,SIGUSR2完成后再继续执行SIGUSR1.

sigaction_nomask_syscall.cpp
SIGUSR1不阻塞SIGUSR2.SIGUSR1在信号处理程序中使用系统调用sleep(这是一个特殊的系统调用),当SIGUSR2在SIGUSR1处理过程中发生,SIGUSR2打断SIGUSR1,SIGUSR2完成后SIGUSR1的sleep直接返回,而非继续等待.
sleep函数时一个非自动重启的函数,它被打断后直接返回剩余的时间.
参考:
https://stackoverflow.com/questions/9661419/sleep-function-will-abort-when-received-a-signal
http://man7.org/linux/man-pages/man7/signal.7.html

struct sigaction {
  void (*sa_handler)(int);
  sigset_t sa_mask;
  int sa_flags;
  void (*sa_sigaction)(int, siginfo_t *, void *);
};

sa_handler: 信号处理函数的地址,SIG_ING和SIG_DEL亦可使用.

sa_mask: 需要屏蔽的信号.在执行此信号的信号处理函数时需要屏蔽哪些信号,即当前信号执行不被打断.此处设计相当合理.信号处理完成之后,屏蔽的信号自然会取消.

sa_flags: 常用的几个标识有SA_INTERRUPT,SA_RESTART,SA_SIGINFO.
SA_INTERRUPT,此信号中断的系统调用将不会自动重启,比如read返回INT的错误.
SA_RESTART,此信号中断的系统调用将自动重启,比如read在信号处理程序结束后会继续等待.
SA_SIGINFO,此信号处理程序使用sa_sigaction而非sa_handler.

sa_sigaction: 当sa_flags设置为SA_SIGINFO时,执行该信号处理程序.注意sa_handler与sa_sigaction可能使用相同的存储,因此sa_sigaction和sa_handler只能只用1个.该字段的信号处理函数,参数siginfo_t包含了信号产生的一些详细信息.
