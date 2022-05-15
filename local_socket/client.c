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

    unlink("client.socket");

    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    syserr(cfd, "socket");


    struct sockaddr_un  client_addr;
    client_addr.sun_family = AF_LOCAL;
    strcpy(client_addr.sun_path, "client.socket");
    int ret = bind(cfd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    syserr(ret, "bind");


    

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.socket");
    ret = connect(cfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    syserr(ret, "connect");

    int num = 0;
    while (1)
    {
        char buf[BUFSIZ];
        sprintf(buf, "helllo I am client, num : %d \n", num++);
        send(cfd, buf, strlen(buf) + 1, 0);
        printf("send msg: %s\n", buf);

        int len = recv(cfd, buf, sizeof(buf), 0);
        syserr(len, "recv");

        if (len == 0) {
            printf("server closed......\n");
            break;
        }
        else if (len > 0)
        {
            printf("reccv from server: %s\n", buf);

        }
        sleep(1);
    }



    close(cfd);



    return 0;
}