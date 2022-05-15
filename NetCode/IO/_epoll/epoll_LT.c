#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

void _syserr(int f_Num, int ret, char* str) {
    if (f_Num == ret) {
        perror(str);
        exit(ret);

    }
}
void syserr(int ret, char* str) {
    if (-1 == ret) {
        perror(str);
        exit(ret);
    }
}


int main() {

    //1.socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    syserr(lfd, "socket");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9527);

    //2.bind
    int ret = bind(lfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    syserr(ret, "bind");

    //3.listen
    ret = listen(lfd, 128);
    syserr(ret, "listen");


    /////////////用epoll_create创建一个epoll实例
    int epfd = epoll_create(1);

    //将监听的文件描述符添加到文件实例中
    struct epoll_event epev;
    epev.events = EPOLLIN /*|EPOLLOUT*/;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    //放置从内核传回来的变动的epev结构体
    struct epoll_event epevs[1024];

    while (1)
    {



        //阻塞监听文件变化，返回变化的文件进入epevs中,返回epevs的数量
        ret = epoll_wait(epfd, epevs, 1024, -1);
        syserr(ret, "epoll_wait");

        printf("ret= %d \n", ret);

        //遍历来查看epevs中每一个的消息
        for (int i = 0;i < ret;i++) {



            int curfd = epevs[i].data.fd;

            if (lfd == epevs[i].data.fd) {
                //如果是客户端，需要调用客户端去 accept，并添加 new cfd into epfd中

                // accept
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);
                int cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);
                syserr(cfd, "accept");

                //将cfd添加到内核rb_root rbr0中
                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }
            else {
                //if (epevs[i].events & EPOLLOUT) continue;

                //不是lfd，则调用 每一个 current cfd 来进行 read & write
                char buf[5] = { 0 };
                int len = read(curfd, buf, sizeof(buf));

                syserr(len, "read");
                if (0 == len) {
                    printf("clident closed..\n");
                    close(curfd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                }
                else if (len > 0) {
                    printf("read buf = %s \n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }


            }
        }
    }


    close(lfd);
    close(epfd);

    return 0;
}