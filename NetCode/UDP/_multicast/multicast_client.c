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


    //加入到多播组
    struct  ip_mreq optval;
    //组播的地址
    inet_pton(AF_INET, "127.0.0.1", &optval.imr_multiaddr.s_addr);
    optval.imr_interface.s_addr = INADDR_ANY;
    setsockopt(cfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &optval, sizeof(optval));

    // reving multicast
    while (1)
    {
        char recvBuf[BUFSIZ];
        char ipBuf[16];
        //receive
        int ret = recvfrom(cfd, recvBuf, strlen(recvBuf), 0, NULL, NULL);

        printf("receive broadcat content : %s\n", recvBuf);

        //printf("%s\n", inet_ntop(AF_INET, INADDR_ANY, ipBuf, sizeof(ipBuf)));

    }

    close(cfd);

    return 0;
}