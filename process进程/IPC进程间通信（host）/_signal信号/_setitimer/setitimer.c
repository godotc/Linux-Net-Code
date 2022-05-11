/*
#include <sys/time.h>

       int getitimer(int which, struct itimerval *curr_value);



       int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);

        功能： 设置闹钟 ，精度更高 ， 可以周期重复触发 interval!=0 

        参数： - which :  以什么来计时
                    - ITIMER_REAL: 真实时间 ,时间到达 发送 SIGALRM ---常用
                    - ITIMER_VIRTUAL: 用户时间
                    - ITIMER_PROF: 以该进程在用户态和内核下所消耗的时间来计算

                - new_value


            struct itimerval {      //间隔结构体
               struct timeval it_interval;     //间隔
                struct timeval it_value;    //延迟多长时间实行定时器 （启动延迟）
           };

           struct timeval {     //时间结构体
               time_t      tv_sec;      //秒数
               suseconds_t tv_usec;     //微秒
           };


            - old_value: 上一次的值 一般为NULL


        返回值： 0 s -1 r
*/

#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

// 过三秒之后 ，每两秒 定时一次
int main() {

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


    getchar();


    return 0;
}