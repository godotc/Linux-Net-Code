/*
    匿名映射： 不需要文件实体进程的

    !! 只能在父子进程之间
*/



#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<wait.h>



int main() {

    //1.创建匿名 内存映射区
    int len = BUFSIZ;
    void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    if (MAP_FAILED == ptr) {
        perror("mmap");
        exit(0);
    }


    //父子进程之间通信

    pid_t pid = fork();
    if (pid > 0) {
        strcpy((char*)ptr, "Hello World!");
        wait(NULL);
    }
    else if (pid == 0)
    {
        sleep(1);
        printf("%s\n", (char*)ptr);
    }

    //释放内存区
    int ret = munmap(ptr, len);
    if (-1 == ret) {
        perror("mumap");
        exit(0);
    }

    return 0;
}