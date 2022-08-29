/*
        #include <unistd.h>

        unsigned int alarm(unsigned int seconds);

        功能 ： 设置定时器（闹钟）
        倒计时 seconds 后会给当前进程发送一个信号：SIGALAM

        取消定时器： 设置alarm为0

        返回值： 倒计时剩余的时间

    - SIGALRM : 默认终止当前的进程， 每一个进程都有且只有唯一一个定时器


        alarm(10);  返回 0
        过了1秒
        alarm(5) >>>> 重置为 5 s   返回 9

    alarm(100) 该函数是不阻塞的
*/


#include<unistd.h>
#include<stdio.h>


int main() {

    //无论进程处于什么状态  都会执行 SIGALARM

    int seconds = alarm(5);

    printf("seconds = %d\n", seconds);//0


    sleep(2);
    seconds = alarm(3);
    printf("seconds = %d\n", seconds);//3

    while (1)
    {

    }



    return 0;
}


