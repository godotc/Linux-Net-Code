/*

    产生的三个条件： 1. 子进程结束 2. 子进程暂停 3. 子进程继续运行
            父进程接接收后  默认 忽略

    解决僵尸进程问题
*/


#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>


void myFun(int num) {
    printf("捕捉到的信d       号: %d \n",num);
    //回收子进程 PCB 的资源
    //while (1) wait(NULL);  wait（NULL） 阻塞的 父进程无法干其他事
    while (1) {
        int ret = waitpid(-1/*所有进   程         */,NULL,WNOHANG);

        if (ret > 0) printf("child die , pid = %d \n", ret);

        else if (ret == 0)break; //还有子进程活着

        else if (ret == -1)break; //没有子进程了
    }
}



int main() {

    //提前设置好阻塞信号集， 阻塞SIGCHILD ， 因为可能子进程 死在父进程注册好之前
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);



    pid_t pid;

    for (int i = 0;i < 20;i++) {
        pid = fork();
        if (pid == 0)break;
    }



    if (pid > 0) {

        //捕捉子进程法术的 SIGCHILD信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        //注册完之后 解除 信号集阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);



        while (1) {
            printf("parent process pid : %d\n", getpid());

            sleep(1);
        }

    }
    else if (pid == 0) {

        printf("child process pid: %d \n", getpid());
        sleep(1);
    }



}