#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main() {

    //1.判断管道文件是否存在
    int ret = access("aFifo", F_OK);
    if (-1 == ret)
    {
        printf("管道不存在，创建相应管道\n");
        ret = mkfifo("aFifo", 0664);
        if (-1 == ret) {
            perror("mkfifo");
            exit(-1);
        }
    }

    ret = access("bFifo", F_OK);
    if (-1 == ret)
    {
        printf("管道不存在，创建相应管道\n");
        ret = mkfifo("bFifo", 0664);
        if (-1 == ret) {
            perror("mkfifo");
            exit(-1);
        }
    }


    //2.以只写方式打开 aFifo
    pid_t wfd = open("aFifo", O_WRONLY);
    if (-1 == wfd) {
        perror("opena");
        exit(-1);
    }
    printf("打开管道aFifo成功，等待写入...\n");

    //3.以只写方式打开 bFifo
    pid_t rfd = open("bFifo", O_RDONLY);
    if (-1 == rfd) {
        perror("openb");
        exit(-1);
    }
    printf("打开管道bFifo成功，等待读取...\n");


    //4.循环写读数据

    char buf[128];

    pid_t pid = fork();

    while (1)
    {

        if (pid > 0) {
            memset(buf, 0, sizeof(buf));

            //获取标准输入的数据
            fgets(buf, 128, stdin);
            //写数据
            ret = write(wfd, buf, strlen(buf));
            if (-1 == ret) {
                perror("write");
                break;
            }

        }
        else if (pid == 0) {
            //5.读管道数据
            memset(buf, 0, 128);
            ret = read(rfd, buf, 128);
            if (-1 == ret) {
                perror("read");
                exit(-1);
            }
            else if (ret == 0) {
                printf("对方已断开....\n");
                break;
            }
            printf("buf : %s \n", buf);
        }
    }


    //6.关闭文件描述符
    close(rfd);
    close(wfd);

    return 0;
}