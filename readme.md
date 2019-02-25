
int chdir(const char* pathname);
成功返回0
支持.. 和.这两个路径名,那么cd 命令就容易实现了.

char *getcwd(char *buf,size_t size);
返回buf