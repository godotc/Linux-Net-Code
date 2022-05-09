/*
    #include <unistd.h>
       #include <fcntl.h>

       int fcntl(int fd, int cmd, ... );

        ...： arg
        cmd: 操作
-F_DUPFD: 复制文件操作符 renturn newfd
-F_GETL 获取指定文件描述符的开状态flag: read write 

-F_SETL: 设置fd 的flag 
    必选： O_RDONLY O_WRONLY O_RDWR
    可选:  O_APPEND, O_NONBLOCK,
        O_NONBLOCK: 设置成非阻塞

阻塞和非阻塞： 函数调用的行为. add()在得到结果后才会返回->赋值 阻塞当前进程 获取结果


*/


#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main() {

    //1.复制文件描述符
    int fd = open("1.txt", O_RDWR);
    if (-1 == fd)perror("open");
    int fd1 = fcntl(fd, F_DUPFD);


    //2.-1 获取文件状态
    int flag = fcntl(fd, F_GETFL);
    flag |= O_APPEND;

    printf("%d", flag);

    //2.更改文件的权限
    int ret = fcntl(fd, F_SETFL, flag);
    if (ret == -1)perror("setfl");

    char* str = "nihao";

    ret = write(fd, str, strlen(str));

    if (-1 == ret)perror("write");

    return 0;
}