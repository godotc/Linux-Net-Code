/*

        字节序：字节在内存中的储存顺序
        大段高位在高地址，小段高位在低地址
*/

//通过代码检测当前主机的字节序


#include<stdio.h>


int main() {

    union {
        short value; //2字节
        char byte[sizeof(short)];   //2字节
    }test;

    test.value = 0x0102;    //0x0102字节

    if ((test.byte[0] == 1) && (test.byte[1] == 2)) {
        printf("大端字节序\n");
    }
    else if ((test.byte[1] == 1) && (test.byte[0] == 2)) {
        printf("小段字节序列\n");
    }
    else {
        printf("未知\n");
    }

    return 0;
}