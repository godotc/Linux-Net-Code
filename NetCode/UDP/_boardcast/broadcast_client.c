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



    //2. 客户端要往这个socket绑定本地的IP和端口，才能接收到广播
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9527);
    addr.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    int ret = bind(cfd, (struct sockaddr*)&addr, sizeof(addr));
    syserr(ret, "bind");

    // reving boardcast
    while (1)
    {
        char recvBuf[BUFSIZ];
        char ipBuf[16];
        //receive
        ret = recvfrom(cfd, recvBuf, strlen(recvBuf), 0, NULL, NULL);

        printf("receive broadcat content : %s\n", recvBuf);

        //printf("%s\n", inet_ntop(AF_INET, INADDR_ANY, ipBuf, sizeof(ipBuf)));

    }

    close(cfd);

    return 0;
}