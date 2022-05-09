/*
#include <signal.h>

        int sigaction(int signum, const struct sigaction* act,struct sigaction* oldact);

        功能： 检查或改变信号的处理，信号捕捉
        参数字：
            act 捕捉之后的处理动作
            oldact 上次动作的设置 一般NULL

        struct sigaction {
               void     (*sa_handler)(int);         //函数指针

               void     (*sa_sigaction)(int, siginfo_t *, void *);  //同上 不常用

               sigset_t   sa_mask;      //临时阻塞信号集------------在信号捕捉函数运行时，临时阻塞某些信号集合

               int        sa_flags;     //指定 用结构体 第一个还是第二个 指针 进行处理 ,0 表示 sa_handler | SA_SIGINFO 表示运用 sa_sigaction

               void     (*sa_restorer)(void);       //废弃
           };
*/


#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>


void myalarm(int num) {
    printf("捕捉到的信号  的编号是:%d\n",num);
    printf("xxxxx\n");
}

// 过三秒之后 ，每两秒 定时一次
int main() {

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);   //清空临时阻塞信号集

    // 注册 信号捕捉
    sigaction(SIGALRM, &act, NULL);


    struct itimerval new_value;

    //2秒间隔
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //3秒延迟
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL); //非阻塞
    printf("定时器开始了\n");

    if (-1 == ret) {
        perror("setitimer");
        exit(0);
    }

    //getchar();
    while (1)
    {
        /* code */
    }


    return 0;
}