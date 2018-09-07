参考:
https://www.ibm.com/developerworks/cn/linux/l-inotifynew/index.html

cat /etc/issue
Ubuntu 14.04.5 LTS \n \l

相关头文件
/usr/include/x86_64-linux-gnu/sys/inotify.h
状态常量,相关结构和函数原型.

注意:监控的是inode,而非文件名.linux以inode作为文件的唯一标识,文件名仅用于容易识别.因此当文件被mv之后,之前添加的监控就会失效,因为此时inode已经发生变化.如果要以文件名监控文件,那么我们需要监控文件的父级目录.


