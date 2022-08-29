/*
NAME
execl, execlp, execle, execv, execvp, execvpe - execute a file

SYNOPSIS
       #include <unistd.h>

       int execl(const char *pathname, const char *arg, ...);
        path: a.out /home/gloria/a.out
        推荐使用绝对路径,进程转变workpath 后可能相对路径不起作用
        args... : main 函数 rgv   
    return:
        成功没有返回值 代码段被替换了 失败 -1
*/

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>

int main() {

    pid_t pid = fork();

    if (-1 == pid) {
        perror("fork");
        return -1;
    }
    else if (pid > 0)
    {
        //father    
        printf("I am father process, pid: %d\n", getpid());
        sleep(1);
    }
    else if (0 == pid)
    {
        //child
        //execl("gloria/hello", "hello", NULL);

        execlp("ps", "ps", "aux", NULL);

        perror("execl");

        printf("I am child process. pid = %d\n", getpid());
    }

    for (int i = 0;i < 3;i++) {

        printf("i = %d , pid = %d\n", i, getpid());
    }



    return 0;
}