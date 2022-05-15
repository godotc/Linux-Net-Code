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



    // server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9527);




    // communicating
    int num = 0;
    while (1)
    {
        // 4. send
        char buf[BUFSIZ];
        sprintf(buf, "hello ,i am clien %d \n", num++);

        int ret = sendto(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        syserr(ret, "sendto");



        //5. receive

         ret = recvfrom(cfd, buf, strlen(buf), 0, NULL, NULL);
         syserr(ret, "recvfrom");

         printf("server say: %s\n", buf);

         sleep(1);
    }

    close(cfd);

    return 0;
}