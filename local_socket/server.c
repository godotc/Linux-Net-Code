// 本地套接字服务器
// strucht sockaddr_un 

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<stdlib.h>

void syserr(int ret, char* str) {
    if (-1 == ret) {
        perror(str);
        exit(-1);
    }
}

int main() {

    unlink("server.socket");

    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    syserr(lfd, "socket");


    struct sockaddr_un  server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.socket");
    int ret = bind(lfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    syserr(ret, "bind");

    ret = listen(lfd, 128);
    syserr(ret, "listen");


    struct sockaddr_un client_addr;
    int len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);
    syserr(cfd, "accept");

    while (1)
    {
        char buf[BUFSIZ];
        int len = recv(cfd, buf, sizeof(buf), 0);
        syserr(len, "recv");

        if (len == 0) {
            printf("client closed......\n");
            break;
        }
        else if (len > 0)
        {
            printf("reccv from client: %s\n", buf);
            send(cfd, buf, strlen(buf) + 1, 0);
        }

    }


    close(cfd);
    close(lfd);



    return 0;
}