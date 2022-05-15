#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

#define IP  127.0.0.1
#define Port 9999

int main() {

    //1.创建sockey(用于监听的套接字)
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //2. 绑定地址与端口
    struct sockaddr_in saddr;

    saddr.sin_family = AF_INET;

    in_addr_t ip = inet_pton(AF_INET, "IP", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = ip;
    //saddr.sin_addr.s_addr = INADDR_ANY; //0.0.0.0 任意地址 0

    in_port_t port = htons(Port);
    saddr.sin_port = port;

    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    //3.监听
    listen(lfd, 128);

    //4.接受客户端链接
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);

    if (-1 == cfd) {
        perror("accept");
        exit(0);
    }

    //输出客户信息
    char clientIP[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    in_port_t clientPort = ntohs(client_addr.sin_port);
    printf("client IP: %s ,Port : %d \n", clientIP, clientPort);


    //5.通信
    char buf[BUFSIZ];

    int retlen = read(cfd, buf, sizeof(buf));
    if (retlen == -1) {
        perror("read");
        exit(-1);
    }
    else if (retlen > 0)
    {
        printf("receive data: %s\n", buf);
    }
    else if (retlen == 0)
    {
        printf("客户端断开链接");
    }




    char* str = "hello client";
    write(cfd, str, sizeof(str));

    close(cfd);
    close(lfd);

    return 0;
}