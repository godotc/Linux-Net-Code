#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

//向管道中写数据
int main() {

    //1. 判断文件是否存在
    if (-1 == access("test_fifo", F_OK)) {
        printf("管道不存在，创建管道\n");
    }

    //2. 创建管道文件
    int ret = mkfifo("test_fifo", 0664);
    if (-1 == ret) {
        perror("mkfifo");
    }

    //3.打开管道        只写
    int fd = open("test_fifo", O_WRONLY);

    //4.写入数据

    // char str = "Hello, i am write process, here is a msg from test_fifo.\n";
    // char buf[BUFSIZ];
    // sprintf(buf, str);

    for (int i = 0;i < 100;i++) {
        char buf[BUFSIZ];
        sprintf(buf, "hello , %d \n", i);
        printf("write data: %s \n", buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }

    close(fd);
    return 0;
}