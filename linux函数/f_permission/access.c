/*
 #include <unistd.h>

int access(const char *pathname, int mode);

    mode:
当前进程是否有 rwx 权限 R_OK|W_OK|X_OK
              F_OK   文件是否存在存在
*/



#include<unistd.h>
#include<stdio.h>



int main() {

    int ret = access("a.txt", X_OK);

    if (-1 == ret) perror("acces");

    printf("文件存在");

}