/*

 #include <signal.h>

       typedef void (*sighandler_t)(int);

       sighandler_t signal(int signum, sighandler_t handler);

            - 功能： 设置某个信号的捕捉行为
            - 参： 
                    - signum ： 要捕捉的信号
                    - hanlder： 如何处理
                            - SIG_IGN : 忽略信号
                            - SIG_DEL : 使用信号默认行为
                            - 回调函数 ： 内核调用 函数指针  程序员只负责写，捕捉到后如何处理

            - 返回值 ： 成功 返回 上一次 注册的信号处理函数的地址， 第一次 NULL
                        失败 返回 SIG_ERR

            SIGKILL SIGSTOP 不能被捕 不能被忽略  
*/


#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>


void myalarm(int num) {
    printf("捕捉到了信号    的编号是:%d\n",num);
    printf("xxxxx\n");
}

// 过三秒之后 ，每两秒 定时一次
int main() {



    // 注册 信号捕捉

    //signal(SIGALRM, SIG_IGN);
    //signal(SIGALRM, SIG_DFT);
    //typedef void (*sighandler_t)(int); int 表示捕捉到的值

    __sighandler_t ret1 = signal(SIGALRM, myalarm);

    if (SIG_ERR == ret1) {
        perror("signal");
        exit(-1);
    }

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