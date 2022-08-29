#include <arpa/inet.h>
#include <cctype>
#include <cstdio>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERV_PORT 12345
#define SERV_IP "127.0.0.1"

int main()
{
    int lfd, cfd;
    char buf[BUFSIZ], client_IP[1024];
    struct sockaddr_in serv_addr, client_addr;

    // socket
    lfd = socket(AF_INET, SOCK_STREAM, 0);

    // bind
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Listen
    ret = listen(lfd, 128);

    socklen_t client_addr_len;
    client_addr_len = sizeof(client_addr);

    // Accept
    cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
    printf("Client IP: %s, Client Port: %d \n", inet_ntop(AF_INET, &client_addr.sin_addr, client_IP, sizeof(client_IP)),
           ntohs(client_addr.sin_port));

    int i, n;

    while (1)
    {
        n = read(cfd, buf, sizeof(buf));
        for (int i = 0; i < n; ++i)
        {
            buf[i] = toupper(buf[i]);
        }

        write(cfd, buf, n);
    }

    close(cfd);
    close(lfd);

    return 0;
}