/*

#include <signal.h>

    int sigprocmask(int how, const sigset_t* set, sigset_t* oldset);

    功能： 将自定义的信号集 设置到 内核中

    参数 : 
        - how : 如何对内核阻塞信号进行处理
            SIG_BLOCK: 将用户设置的阻塞信号添加到内核中原来的数据不变 mask|set

            SIG_UNBLOCK: 根据用户设置的数据， 对内核中的数据进行 解除 阻塞 msk & = (^set)

            SIG_SETMASK: 覆盖内核中原来的值

        -oldset : 保存的之前内核中的值(NULL)
    返回值 0 -1 
        错误号EFAULT EINVAL



    int sigpending(sigset_t *set);
        功能： 获取内核中的未决信号集



    int rt_sigprocmask(int how, const kernel_sigset_t* set, kernel_sigset_t* oldset, size_t sigsetsize);


    int sigprocmask(int how, const old_kernel_sigset_t* set, old_kernel_sigset_t* oldset);

*/


//programing a program ， 把常规信号（1-31）的味觉状态打印到屏幕

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

int main() {

    //设置 2、3 信号阻塞
    sigset_t set;
    sigemptyset(&set);

    //添加
    if (-1 == sigaddset(&set, SIGQUIT)) {
        perror("setaddQUIT");
        exit(0);
    }
    if (-1 == sigaddset(&set, SIGINT)) {
        perror("setaddINT");
        exit(0);
    }



    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);

    int num = 0;

    while (1)
    {
        //获取当前的未决信号集的数据
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        //编辑前32 为
        for (int i = 1;i <= 31; i++) {
            if (sigismember(&pendingset, i) == 1) {
                printf("1");
            }
            else if (sigismember(&pendingset, i) == 0) {
                printf("0");
            }
            else {
                perror("sigismember");
                exit(0);
            }

        }
        printf("\n");
        sleep(1);

        num++;
        if (num == 10) {
            //解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }



    return  0;
}