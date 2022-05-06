

/*
 ssize_t read(int fd, void *buf, size_t count);
  cout == size of buf 指定数组大小
   buf 需要读数据存储位置，（传出参数）；
 返回值：>0读取到的字节数量| —1 error |0 文件已经读取读完了 

ssize_t write( int fd, const void *buf, size_t count);

返回值: 
*/

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main() {

    int fd = open("a.txt", O_RDWR);
    if (-1 == fd) {
        perror("open filed");
        return -1;
    }

    int cpfd = open("./copy.txt", O_CREAT | O_WRONLY, 0664);
    if (-1 == cpfd) {
        perror("creat or open");
        return -1;
    }

    char buf[1024] = { };

    int len = 0;
    while ((len = read(fd, buf, sizeof(buf))) > 0)
    {
        write(cpfd, buf, len);
    }
    if (-1 == len) perror("read");

    close(fd);
    close(cpfd);


    return 0;
}