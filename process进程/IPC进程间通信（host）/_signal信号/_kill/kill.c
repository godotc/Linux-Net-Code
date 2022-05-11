/*

        #include <sys/types.h>
        #include <signal.h>

         int kill(pid_t pid, int sig);

        - 功能： 给某个进程pid，发送 signal
        - sig : 信号编号（最好使用宏值，不同架构信号量不同）
        - pid : <-1   某一个进程组的id(外部)
                >0   发送给指定的进程
                == 0  发送给当前的进程组
                == -1 将信号发送给每一个有权限接受这个信号的进程


        int raise(int sig): 给当前进程发送信号
            返回值： 0成功 非0 失败
        kill(getpid(),sig)

        void abort(void): 发送 SIGABRT给当前进程，杀死当前进程
        kill(getpid(),SIGABRT)

*/

#include<stdio.h>
#include<sys/types.h>
#include<signal.h>  
#include<unistd.h>


int main() {

    pid_t pid = fork();

    if (0 == pid) {

        int i = 0;
        for (i = 0;i < 5;i++) {
            printf("Child process\n");
            sleep(1);
        }
    }else if (pid>0)
    {
        printf("Parent process\n");
        sleep(2);
        printf("Kill child process now\n");
        kill(pid, SIGINT);
    }


    return 0;
}
