//1秒钟电脑能数多少个数？


#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>


/*
    实际时间 = 内核时间 + 用户时间 + 消耗的时间
    执行 IO 比较消耗时间

*/

int main() {



    alarm(1);
    int i = 1;



    while (1)
    {
        printf("%d\n", i++);
    }


    return 0;
}