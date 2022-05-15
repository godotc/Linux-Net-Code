#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void syserr(int ret, char* str) {
    if (-1 == ret) {
        perror(str);
        exit(ret);
    }
}

int main() {

    //1.socket
    int cfd = socket(AF_INET, SOCK_DGRAM, 0); //0默认udp
    syserr(cfd, "socket");


    //2. 设置多播的属性（外出接口）

    struct in_addr imr_multiaddr;
    //初始化多播地址
    inet_pton(AF_INET, "127.0.0.1", &imr_multiaddr);
    setsockopt(cfd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));


    //3.初始化客户端的地址信息
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(9527);
    inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr.s_addr);

    // boardcast

    int num = 0;
    while (1)
    {
        // send
        char sendBuf[BUFSIZ];
        sprintf(sendBuf, "hello , client ........%d\n", num++);

        sendto(cfd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

        printf("multicast content: %s\n", sendBuf);
        sleep(1);

    }

    close(cfd);
    return 0;
}