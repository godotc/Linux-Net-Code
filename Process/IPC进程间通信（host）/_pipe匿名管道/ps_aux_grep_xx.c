/*

    实现 ps aux | grep xxx 父子进程间通信

    子进程： px aux， 子进程结束后，将数据发给父进程
    父进程： 获得数据 过滤

    pipe()
    execlp()
    子进程将标准输出stdout_fileno 重定向到写端. dup2

*/

#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<wait.h>

int main() {

    //创建管道
    int fd[2];
    int ret = pipe(fd);


    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();


    if (pid > 0) {
        //父

        //关闭写端
        close(fd[1]);

        //读取数据
        char buf[BUFSIZ];
        int len = -1;


        //过滤输出
        while (len = read(fd[0], buf, sizeof(buf) - 1)) {
            if (-1 == len) perror("read");
            printf("%s", buf);
            memset(buf, 0, BUFSIZ);
        }

        wait(NULL);
    }
    else if (0 == pid) {
        //子

        //关闭读端
        close(fd[0]);

        //文件描述符重定向 把stdout_fileno -> fd[1]
        dup2(fd[1], STDOUT_FILENO);


        //执行 ps aux      execlp调用系统函数
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);
    }
    else {
        perror("fork");
        exit(0);
    }

    return 0;
}