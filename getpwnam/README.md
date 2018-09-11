getpwnam可获取指定用户的相关信息,其内部使用了全局数据结构,因此为不可重入方法.

safe_check.cpp
以SIGALRM中断不可重入函数getpwnam,在Ubuntu14.04中产生死锁.

