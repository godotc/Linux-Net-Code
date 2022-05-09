/*

    1. 通过命令 mkfifo -n
    2. 通过函数 int mkfifo
        #include <sys/types.h>
        #include <sys/stat.h>

        int mkfifo(const char *pathname, mode_t mode);

        - mode:文件的权限,和open的mode是一样的

*/


#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {

    //判断文件是否存在
    if (-1 == access("f_fifo", F_OK)) {
        printf("管道不存在，创建管道\n");
    }


    int ret = mkfifo("f_fifo", 0664);
    if (-1 == ret) {
        perror("mkfifo");
        exit(0);
    }
    return 0;
}