#include<arpa/inet.h>
#include<stdio.h>


int main() {

    //创建一个IP字符串
    char buf[] = "192.168.1.1";
    unsigned int dst = 0;

    //将点分十进制的IP字符串转换为网络字节序的整数
    inet_pton(AF_INET, buf, &dst);
    unsigned char* p = (unsigned char*)&dst;
    printf("%d %d %d %d\n", *p, *(p + 1), *(p + 2), *(p + 3));

    //将网络字节序的整数转换为点分十进制IP字符串
    char ip[16] = "";

    char* str = inet_ntop(AF_INET, &dst, ip, 16);

    printf("str = %s\n", str);
    printf("ip = %s\n", ip);

    return 0;
}