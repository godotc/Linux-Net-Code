// NAME
// stat, fstat, lstat, fstatat - get file status

// SYNOPSIS
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>

// int stat(const char* pathname, struct stat* statbuf);

//   statbuf: 结构体变量，传出参数，缓冲区 保存读取到的文件信息

// int lstat(const char* pathname, struct stat* statbuf);


// int fstat(int fd, struct stat* statbuf);
// int fstatat(int dirfd, const char* pathname, struct stat* statbuf, int flags);



#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

struct stat a;

int main() {

    struct stat statbuf;
    if (-1 == stat("a.txt", &statbuf)) {
        perror("stat");
        return -1;
    }

    printf("size:%ld\n", statbuf.st_size);
    //printf("Current user mod:%d\n", statbuf.st_mode & 0000000111000000);

    return 0;
}