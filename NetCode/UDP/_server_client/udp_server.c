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


    //2.bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //inet_pton(AF_INET, INADDR_ANY, &addr.sin_addr.s_addr);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9527);
    int ret = bind(cfd, (struct sockaddr*)&addr, sizeof(addr));
    syserr(ret, "bind");


    // communicating

    while (1)
    {
        // 4. receive
        char buf[BUFSIZ];

        char ipBuf[16];
        struct sockaddr_in client_addr;
        int len = sizeof(client_addr);

        int ret = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &len);

        syserr(ret, "recvfrom");
        if (0 == ret) {
            printf("EOF or empty package");  //读完文件，或者接收的是空消息
        }

        //client info
        printf("client IP: %s\n, Prot: %d \n",
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipBuf, sizeof(ipBuf)),
            ntohs(client_addr.sin_port));

        printf("client say: %s\n", buf);

        //5. send

        sendto(cfd, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

    }

    close(cfd);

    return 0;
}