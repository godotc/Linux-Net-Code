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


    //2. 设置广播属性       //不需要bind 因为只需要发送数据
    int optval = 1;
    setsockopt(cfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));


    //3.创建 广播的地址
    struct sockaddr_in broadcast_addr;
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(9527);
    inet_pton(AF_INET, "172.31.223.70", &broadcast_addr.sin_addr.s_addr);

    // boardcast

    int num = 0;
    while (1)
    {
        // send
        char sendBuf[BUFSIZ];
        sprintf(sendBuf, "hello , client ........%d\n", num++);

        sendto(cfd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));

        printf("brocast content: %s\n", sendBuf);
        sleep(1);

    }

    close(cfd);
    return 0;
}