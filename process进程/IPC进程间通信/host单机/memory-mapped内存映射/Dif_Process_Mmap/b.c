#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/stat.h>
#include<wait.h>

int main() {

    int Wfd = open("./contactFile/bwar.txt", O_RDWR);
    if (-1 == Wfd) {
        perror("open");
        exit(0);
    }
    int sizeW = lseek(Wfd, 0, SEEK_END);

    int Rfd = open("./contactFile/awbr.txt", O_RDWR);
    if (-1 == Rfd) {
        perror("open");
        exit(0);
    }
    int sizeR = lseek(Rfd, 0, SEEK_END);

    /////////////////////////////////////////////////////////////////////////////////////////////

    void* Wptr = mmap(NULL, sizeW, PROT_READ | PROT_WRITE, MAP_SHARED, Wfd, 0);
    if ((void*)-1 == Wptr) {
        perror("mmapW");
        exit(-1);
    }

    void* Rptr = mmap(NULL, sizeW, PROT_READ | PROT_WRITE, MAP_SHARED, Rfd, 0);
    if ((void*)-1 == Rptr) {
        perror("mmapW");
        exit(-1);
    }


    pid_t pid = fork();

    while (1)
    {
        if (pid == -1) {
            perror("fork");
            exit(-1);
        }
        else if (pid > 0) {
            //父进程 写
            char buf[128];
            scanf("%s", &buf);
            strcpy((char*)Wptr, buf);
            sleep(1);
            wait(NULL);
        }
        else if (pid == 0)
        {
            //子进程 读
            char buf[128];
            strcpy(buf, (char*)Rptr);
            printf("Read data：%s \n     ",buf);
            sleep(1);
        }

    }

    munmap(Rptr, sizeR);
    munmap(Wptr, sizeW);



    return 0;
}