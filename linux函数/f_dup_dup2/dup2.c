/*
     int dup2(int oldfd, int newfd);
    int ret = dup2(pd,newpd);

    作用：重定向文件描述符


    #define _GNU_SOURCE
    #include <fcntl.h>
    #include <unistd.h>

    int dup3(int oldfd, int newfd, int flags);

*/