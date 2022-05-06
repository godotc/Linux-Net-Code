#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main() {

    int fd = open("a.txt", O_RDONLY);

    if (-1 == fd) {
        perror(" ");
    }

    close(fd);


    //第三个参数 权限 mode & ~umask 抹去有些权限 （将文件的权限与 mask相与 &),rwxrwx不为1的就会被排除掉
    // 0777 -> 111111111
    // 0775 -> 111111101
    // r^2 + w^1 + x ^0   4+2+1 |4+2+0| 4+0+1| 0+2+1|
    //                       7      6     5      3
    //mask 一个 mode_t 8进制数字
    int fd2 = open("a.txt", O_RDONLY | O_RDWR | O_APPEND, 0777);

    //第二个参数 是 int 4byte 32bit 与的话 | 就把为1的权限加上去

    
    
    return 0;
}