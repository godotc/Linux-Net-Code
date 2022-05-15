#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

void serr(char* str) {
    perror("str");
    exit(-1);
}

int main() {

    //1.创建套接字
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == cfd)serr("socket");

    //2.Connect
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9527);

    int ret = connect(cfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (-1 == ret)serr("connect");

    //3. Write & Read
    while (1)
    {
        char* str = "hello I am client";
        write(cfd, str, strlen(str) + 1);

        char buf[BUFSIZ] = { 0 };

        int len = read(cfd, buf, sizeof(buf));

        if (-1 == len)serr("read");

        else if (len > 0) printf("receive server: %s\n", buf);

        else if (0 == len)
        {
            printf("server closed...\n");
            break;
        }

        sleep(1);
    }

    //4.close
    close(cfd);

    return 0;
}