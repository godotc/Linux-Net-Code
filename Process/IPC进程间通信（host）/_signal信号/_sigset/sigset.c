/*

#include <signal.h>

        int sigemptyset(sigset_t *set);
        功能： 清空信号集中的数据，全部置为0
            -1 0

        int sigfillset(sigset_t *set);
        功能 ： 将信号集中 所有的标志位置为1；
            0 -1

        int sigaddset(sigset_t *set, int signum);
        功能： 设置信号集中一个标志位为 1， 表示阻塞这个信号

        int sigdelset(sigset_t *set, int signum);
         功能： 设置信号集中一个标志位为 0， 表示不阻塞这个信号 

        int sigismember(const sigset_t *set, int signum);
        功能： 判断 某个信号是否阻塞（标志位是否为1)
        0 -1
*/


#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>



int main() {

    //创建一个信号集
    sigset_t set;

    //初始化信号机
    sigemptyset(&set);


    //判断 SIGINT 是否在信号集 set 里
    int ret = sigismember(&set, SIGINT);
    if (0 == ret)printf("SIGINT 不阻塞\n");
    else if (1 == ret)printf("SIGINT 阻塞\n ");


    //添加几个信号到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);


    //判断 SIGINT 是否在信号集 set 里
    ret = sigismember(&set, SIGINT);
    if (0 == ret)printf("SIGINT 不阻塞\n");
    else if (1 == ret)printf("SIGINT 阻塞\n");



    //从信号集中删除信号
    sigdelset(&set, SIGINT);
    //判断 SIGINT 是否在信号集 set 里
    ret = sigismember(&set, SIGINT);
    if (0 == ret)printf("SIGINT 不阻塞\n");
    else if (1 == ret)printf("SIGINT 阻塞\n");


    return 0;
}