// 使用内存映射实现文件拷贝

/*

    思路：
    1.对原始文件进行内存映射
    2.创建一个新文件（拓展该文件）
    3.把新文件的数据映射到内存中
    4.通过内存拷贝将第一个文件的内存数据拷贝到内存中
    5.释放资源

*/


#include<stdio.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main() {

    //1.对原始文件 进行内存映射
    int fd = open("english.txt", O_RDONLY);
    if (-1 == fd) {
        perror("open");
        exit(0);
    }

    //1.5 获取原始文件大小
    int size_o = lseek(fd, 0, SEEK_END);


    //2.创建一个新文件，对其拓展
    int nfd = open("cp.txt", O_RDWR | O_CREAT, 0664);
    if (-1 == nfd) {
        perror("open1");
        exit(0);
    }

    //2.5 对新文件进行拓展
    truncate("cp.txt", size_o);
    write(nfd, " ", 1);


    //3. 分别做内存映射 
    void* ptr1 = mmap(NULL, size_o, PROT_READ, MAP_SHARED, fd, 0);
    void* ptr2 = mmap(NULL, size_o, PROT_READ | PROT_WRITE, MAP_SHARED, nfd, 0);

    if (MAP_FAILED == ptr1) {
        perror("mmap1");
        exit(0);
    }
    if (MAP_FAILED == ptr2) {
        perror("mmap2");
        exit(0);
    }


    //4. 内存拷贝
    memcpy(ptr2, ptr1, size_o);


    //5.释放资源
    munmap(ptr1, size_o);
    munmap(ptr2, size_o);

    close(fd);
    close(nfd);

    return 0;
}