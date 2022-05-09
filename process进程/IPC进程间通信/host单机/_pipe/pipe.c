/*
    #include <unistd.h>


    struct fd_pair {
         long fd[2];
    };
    struct fd_pair pipe();

    int pipe(int pipefd[2]);


    创建一个管道
    - 参数：
        pipefd[0] 管道 写端
        pipefd[1] 读端
    - 返回: 0 s -1 r

    !! 只能 用于 有关系的进程间通信
    !! 管道默认是阻塞的，没有数据 read 阻塞 ，满了，write阻塞
    !! 一般是单向的

    #define _GNU_SOURCE
    #include <fcntl.h>
    #include <unistd.h>

    int pipe2(int pipefd[2], int flags);

*/


//子进程发送数据给父进程，父进程读到后输出

#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
int main() {

    //在fork之前创建管道
    int pipefd[2];
    if (-1 == pipe(pipefd)) {
        perror("pipe");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        //父进程

        printf(" I am father process, pid : %d\n", getpid());

        //如果父进程只读 -> 关闭写端
        close(pipefd[1]);

        char buf[BUFSIZ];
        while (1) {
            // //写
            // char* str = "Hello,i am father";
            // write(pipefd[1], str, strlen(str));
            // //sleep(1);

            //读        需要在写下面 如果 两个进程 都是 读 就会 都阻塞
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("Father receive: %s , pid : % d \n", buf, getpid());
        }
    }
    else if (pid == 0) {
        //子进程       
        printf("I am child process, pid : %d \n", getpid());

        //只写   ->关闭读端
        close(pipefd[0]);

        char buf[BUFSIZ];
        while (1) {
            // //读 
            // int len = read(pipefd[0], buf, sizeof(buf));
            // printf("Child receive: %s , pid : % d \n", buf, getpid());

            //写
            char* str = "Hello,i am child";
            write(pipefd[1], str, strlen(str));
            memset(buf, 0, sizeof(buf));
            //sleep(1);
        }
    }

    return 0;
}