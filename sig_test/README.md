#配置ulimit设置core文件大小
ulimit -c 10000

如果遇到错误"cannot modify limit: Operation not permiteed.",可如下修改:
sudo vim /etc/ssh/sshd_config
找到UserLogin将no修改为yes,然后将注释放开.Ubuntu14.04.5如下重启ssh:
sudo stop ssh
sudo start ssh
然后重新执行ulimit -c 10000

这种修改只能在当前会话中生效,对于该用户长久生效的方法如下:
vim ~/.bash_profile,在末尾增加
ulimit -c 10000
source ~/.bash_profile即可生效.

针对全系统的所有用户修改如下:
sudo vim /etc/profile

参考:
https://www.cnblogs.com/rusking/p/5563761.html

