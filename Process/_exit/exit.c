#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>


int main() {

    printf("hello \n");
    printf("world");

    // exit(0);  //带有 刷新缓冲区 函数 fflush
    
    //fflush(stdout);
    _exit(0);   //没有打印world 

    return 0;
}