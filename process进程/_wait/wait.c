/*
NAME
wait, waitpid, waitid - wait for process to change state

SYNOPSIS
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *wstatus);

        - wstatus: 进程退出时的转台信息 int类型的地址

        - 返回： 成功 返回子进程id

        调用wait函数的进程 会 suspend （阻塞），知道他的一个子进程退出 或者 收到 一个不能忽略的信号
        此时被唤醒继续执行
        > 如果没有子进程 return -1 
        > 如果子进程结束 也 return -1 并回收资源



       pid_t waitpid(pid_t pid, int *wstatus, int options);

       int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
*/

#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main() {

    pid_t pid;

    //fork();
    //fork();  3个进程

    for (int i = 0; i < 5;i++) {
        pid = fork();
        if (pid == 0) break;
    }

    if (pid > 0) {
        //父进程
        for (;;) {

            int st;
            int ret = wait(&st);

            printf(" parent , pid = %d\n", getpid());

            //int ret = wait(NULL);


            if (ret == -1) break;   //子进程都结束完了 停止回收


            if (WIFEXITED(st)) {
                //是不是是正常退出
                printf("退出的状态    码: %d\n",WEXITSTATUS(st));
            }
            if (WIFSIGNALED(st)) {
                //是不是异常终止
                printf("被哪个信号干    掉了: %d\n",WTERMSIG(st));       //kill -9 返回 9信号
            }


            printf("child die, pid = %d \n", ret);

            sleep(1);
        }
    }
    else if (pid == 0)
    {
        //子进程    
        for (;;) {
            printf(" child , pid = %d\n", getpid());
            sleep(1);
        }
        exit(0);
    }


    return 0;
}