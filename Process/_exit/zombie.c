
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>


int main() {
    int num = 10;


    pid_t pid = fork();


    if (pid > 0) {
        //父进程
        while (1)
        {
            //子进程死了 父进程还在继续  子进程的  pcb 没有人释放 -> 占用 pid  
                // --- 
                // --- 但是一般又不能结束主进程来杀死僵尸进程
            printf("I am father,my pid is : %d , ppid : %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else if (pid == 0) {
        //子进程       
        printf("I am child,my pid is : %d , ppid : %d\n", getpid(), getppid());

    }
    else if (pid == -1)
    {
        perror("- fork");
        return -1;
    }

    for (int i = 0;i < 3;i++) {
        printf("i:%d pid:%d \n", i, getpid());
    }


    return 0;
}
