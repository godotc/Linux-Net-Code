#include<stdio.h>
#include<arpa/inet.h>


int main() {

    //htons 转换端口
    unsigned short a = 0x0102;
    unsigned short b = htons(a);
    printf("a: %x\n", a);
    printf("b: %x\n", b);

    //htonl 转换IP
    char buf[4] = { 192,168,1,100 };
    int num = *(int*)buf;
    int ans = htonl(num);

    unsigned char* p = (char*)&ans;

    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));

    //ntohl 
    unsigned char buf1[4] = { 1,1,168,192 };
    int num1 = *(int*)buf1;
    int ans1 = ntohl(num1);
    unsigned char* p1 = (unsigned char*)&ans1;
    printf("%d %d %d %d\n", *p1, *(p1 + 1), *(p1 + 2), *(p1 + 3));



    return 0;
}