
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>


int main() {
    int num = 10;


    pid_t pid = fork();


    if (pid > 0) {
        //父进程
        printf("I am father,my pid is : %d , ppid : %d\n", getpid(), getppid());
    }
    else if (pid == 0) {
        //子进程
        sleep(1);
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
