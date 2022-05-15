#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    inet_pton(AF_INET, "192.168.193.128", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9999);

    connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    write(fd, "hello , i am client", sizeof("hello , i am client"));

    char buf[BUFSIZ];
    int retlen = read(fd, buf, sizeof(buf));
    if (retlen = -1) {
        perror("read");
        exit(-1);
    }
    else if (retlen > 0)
    {
        printf("receive data: %s\n", buf);
    }
    else if (retlen == 0)
    {
        printf("服务器断开链接");
    }

    close(fd);

    return 0;
}