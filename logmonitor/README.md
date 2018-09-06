input.cpp
./input <filename>
当filename被rename或者delete时,input可以自动重新打开新的filename继续记录内容.

output.cpp
./output <filename>
类似于tail -f的功能.当filename被删除,然后重新写入时output也支持.
