/*
NAME
       fork - create a child process

SYNOPSIS
       #include <sys/types.h>
       #include <unistd.h>

       pid_t fork(void);

    返回值: 
        -1 创建失败
        0  子进程中返回 0
        >0  父进程中 返回 子进程的pid
        用返回值 区分 父子进程

> 实际上 fork 是通过 写时拷贝(copy- on - write); 资源共享只读 -> 需要写文件时 拷贝 需要写的资源 进入 自己 的虚拟地址空间


> 父子进程对变量是不是共享的?
 - 刚开始是共享的， 修改数据后（写）后就不共享了

*/
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>


int main() {
    int num = 10;

    //创建子进程--------子进程包含前面的文件 但只执行 fork 行后的代码
    pid_t pid = fork();



    //判断是父进程还是子进程
    if (pid > 0) {
        //父进程

        printf("parent num = %d\n", num);
        num += 10;
        printf("parent num +=10 ： %d\n", num);

        printf("- pid:%d\n", pid);
        printf("I am father,my id is:%d,ppid:%d\n", getpid(), getppid());
    }
    else if (pid == 0) {
        //子进程


        printf("child num = %d\n", num);
        num += 100;
        printf("child num +=100 ： %d\n", num);


        printf("- pid:%d\n", pid);
        printf("I am child,my id is:%d,my father id is:%d\n", getpid(), getppid());
    }
    else if (pid == -1)
    {
        perror("- fork");
        return -1;
    }

    // for (int i = 0;i < 100;i++) {
    //     printf("i:%d pid:%d\n", i, getpid());
    // }


    return 0;
}
