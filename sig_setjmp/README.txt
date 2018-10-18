当捕捉到一个信号时,进入信号的捕捉函数,此时当前信号会被自动加入到进程的信号屏蔽字中.

如果在信号处理程序中调用setjmp和longjmp,setjmp和longjmp不能在跳转时选择是否恢复屏蔽字的.
sigsetjmp和siglongjmp可选择在跳转时是否恢复屏蔽字.

sigsetjmp 参数2非0时会保存当前进程的信号屏蔽字到参数1中.
siglongjmp 参数2非0时,跳转出去时会恢复sigsetjmp保存的信号屏蔽字(如果sigsetjmp未保存则无法恢复).

sigsetjmp_mask & siglongjmp_mask == 1 => 恢复屏蔽字
                                 != 1 => 不恢复屏蔽字

因此在信号处理程序中使用setjmp/longjmp,sigsetjpm/siglongjmp跳转时务必注意是否恢复屏蔽字带来的问题.
