getpwnam可获取指定用户的相关信息,其内部使用了全局数据结构,因此为不可重入方法.

safe_check.cpp

