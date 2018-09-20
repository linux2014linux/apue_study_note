#ALARM信号终止其他信号处理
g++ alarm_int_other.cpp -o alarm_int_other


alarm 在指定秒后产生1个SIGALRM信号;
pause 当任一信号的处理程序执行完成时,pause返回;


程序执行后,立即按下Ctrl+c,整个执行流程如下:

pause阻塞;
SIGINT产生,sig_int执行;

如果sig_int在SIGALRM产生之前完成,pause返回,sig_alrm返回alarm的剩余值;
如果sig_int在SIGALRM产生之后完成,SIGALRM产生时会中断sig_int而执行sig_alrm,sig_alrm执行完成后pause返回,my_sleep返回,程序执行完成.此时的sig_int就未被处理了.

关于信号的处理程序,必须非常谨慎,细节真的太多.
