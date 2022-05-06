/*
    #include <sys/types.h>
    #include <unistd.h>

    off_t lseek(int fd, off_t offset, int whence);

        - offset 偏量 
        - whence:
            SEEK_SET 设置文件指针的偏移量
            SEEK_CUR  当前位置+第二参数offset的值  
            SEEK_END    文件大小+第二个参数 offset的值

            1. 移动文件指针到文件头   lseek (fd,0,SEEK_SET);
            2. 获取当前文件指针的位置   lseek(fd,0,SEEK_CUR);
            3. 获取文件长度             lseek(fd,0,SEEK_END);

*/

//4. 拓展文件的长度  lseek (fd, 100, SEEK_END);
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc) {

    int fd = open("hello.txt", O_RDWR);

    if (-1 == fd) {
        perror("open");
        return -1;
    }

    //拓展文件长度
    int ret = lseek(fd, 100, SEEK_END);
    if (-1 == ret) {
        perror("lseek");
        return -1;
    }

    //写入一个空数据

    write(fd, " ", 1);


    close(fd);

    return 0;
}