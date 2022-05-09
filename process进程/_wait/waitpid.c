/*
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int* wstatus);

 - pid_t waitpid(pid_t pid, int* wstatus, int options);


可以回收指定进程 可以设置非阻塞

    参数：
        - pid :
            >  0  : 某个子进程的pid
            =  0  ：回收当进程的所有子进程(当前组的子进程)
            = -1  : 回收所有的子进程 ==  wait()  （所有组的其他组的都回收）
            < -1  : 回收某个进程组 组的 id 等于 这个参数的绝对值
        - options ：
            设置阻塞或者非阻塞
                0 : 阻塞的
            WNOHANG: 非阻塞
    返回值：
        > 0 : 返回子进程id
        = 0 ： options = WNOHANG 表示还有子进程
        - 1 ： wrong

*/
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main() {

    pid_t pid;

    for (int i = 0; i < 5;i++) {
        pid = fork();
        if (pid == 0) break;
    }


    if (pid > 0) {
        //父进程

        for (;;) {


            int st;

            // int ret = waitpid(-1, &st, 0);
            int ret = waitpid(-1, &st, WNOHANG);


            printf(" parent , pid = %d\n", getpid());
            sleep(1);

            if (ret == -1) {
                break;   //子进程都结束完了 停止回收
            }
            else if (0 == ret) {
                //说明还有子进程存在
                continue;
            }
            else if (ret > 0) {

                if (WIFEXITED(st))
                {
                    //是不是是正常退出
                    printf("退出的状态   码: %d\n",WEXITSTATUS(st));
                }
                if (WIFSIGNALED(st))
                {
                    //是不是异常终止
                    printf("被哪个信号干   掉了: %d\n",WTERMSIG(st));       //kill -9 返回 9信号
                }

            }

            printf("-----------------child die, pid = %d \n", ret);


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