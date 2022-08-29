/*
        #include <sys/mman.h>

       void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
        - 功能：
            将一个文件或设备的数据映射到内存中
        - parameeter:
                - addr  :  NULL 由内核指定
                - length:  映射的数据长度,不能为0，建议用文件的长度(stat lseek)
                - prot  ： 对申请的内存映射区的操作权限
                            PROT_EXEC/READ/WRITE/NONE
                - flags :   MAP_SHARED/PRIVATE : 是否会与磁盘文件同步，进程间通信必要设置共享
                - fd    :   要映射的那个 文件的文件描述符》通过 open得到
                            》权限不能与prot冲突（含于PROT)》 大小不能为0
                - offset:   偏移量， 必须是4k的整数倍 ，一般不用，为0

       int munmap(void *addr, size_t length);
            unmaP :释放存映射 
            首地址 + 长度 
            成功放回 ： 一个指针指向头
            失败返回    (void *)-1


*/

/*
    使用内存映射实现进程间通信：
    1. 有关系的进程（父子）：
            - 还没有子进程的时候： 通过唯一的父进程，创建内存映射区
            - 有了内存应摄区后， 创建子进程
            - 父子进程共享创建的内存映射区

    2. 没有关系的
            - 准备一个大小不是0的磁盘文件
            -进程 1 ：创建映射区，得到一个指针
            -进程2： 创建-》指针
            -使用内存映射区通信
    注意： 内存映射区，是非阻塞的
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

    //1.打开一个文件
    int fd = open("test.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);  //获取文件大小


    //2.创建内存映射区
    void* ptr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //3.创建子进程
    pid_t pid = fork();

    if (pid == 0) {
        //子进程
        strcpy((char*)ptr, "Hello, father!!!");

    }
    else if (pid > 0) {
        //父进程
        wait(NULL);
        char buf[64];
        strcpy(buf, (char*)ptr);
        printf("read data : %s\n", buf);

    }


    //4.关闭内存映射区
    munmap(ptr, size);


    return 0;
}