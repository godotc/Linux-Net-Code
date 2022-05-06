/*
    #include <unistd.h>

    int dup(int oldfd);


    作用:复制一个新的文件描述符
    int newpd = dup(pd);

   

*/

#include<unistd.h>
#include<stdio.h>
#include<string.h>

#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main() {

    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);
    printf("old fd: %d\n", fd);

    int fd1 = dup(fd);

    if (fd1 == -1) {
        perror("dup");
        return -1;
    }

    printf("new fd1:%d\n", fd1);

    //关闭fd 后 fd1 仍指向同一文件 可以操作同一文件
    close(fd);

    char* str = "hello world";

    if (-1 == write(fd1, str, strlen(str))) {
        perror("write");
        return -1;
    }


    close(fd1);

    return 0;
}