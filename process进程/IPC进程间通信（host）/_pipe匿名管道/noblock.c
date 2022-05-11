#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

/*
设置管道非阻塞

int fcntl(int fd,cmd)


int flags = fcntl(fd[0],F_GETFL);   //获取flag
flags|=O_NONBLOCK;       //修改flag的值
fcntl(fd[0],F_SETFL,flags);     //修改读端属性

*/


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

        printf("I am father process, pid : %d\n", getpid());

        int flags = fcntl(pipefd[0], F_GETFL);   //获取flag
        flags |= O_NONBLOCK;                    //修改flag的值
        fcntl(pipefd[0], F_SETFL, flags);     //修改读端属性



        //关闭写端
        close(pipefd[1]);

        char buf[BUFSIZ];

        while (1)
        {
            //读        
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len: %d \n", len);
            printf("Father receive: %s , pid : % d \n", buf, getpid());
            memset(buf, 0, BUFSIZ);
            sleep(2);
        }
    }

    else if (pid == 0) {
        //子进程       
        printf("I am child process, pid : %d \n", getpid());

        //关闭读端
        close(pipefd[0]);

        char buf[BUFSIZ];

        while (1)
        {
            //写
            char* str = "Hello,i am child";
            write(pipefd[1], str, strlen(str));
            //memset(buf, 0, sizeof(buf));
            sleep(5);
        }
    }

    return 0;
}