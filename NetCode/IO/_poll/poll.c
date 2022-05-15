#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/poll.h>

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

    ////初始化检测的文件描述符数组
    struct pollfd fds[1024];
    for (int i = 0;i < 1024;i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }

    fds[0].fd = lfd;

    int nfds = 0;

    while (1)
    {
        //调用POLL系统函数
        ret = poll(fds, nfds + 1, -1);   //-1表示阻塞    

        if (0 == ret) {
            //意思是没有检测到有数据，但是设置永久阻塞，不可能返回为0
            continue;
        }
        else if (ret > 0) {

            //检测到有文件描述符发生了改变
            if (fds[0].revents & POLLIN)    //只监听POLLIN
            {
                //表示有新的客户端链接进来了
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);

                //4. accept
                int cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);

                //将新的文件描述符加入到集合中
                for (int i = 1;i < 1024;i++) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                //更新右区间（最大的文件描述符）
                nfds = cfd > cfd ? nfds : cfd;
            }

            for (int i = 1;i <= nfds; i++) {
                if (fds[i].revents & POLLIN) {

                    //说明这个文件描述符的对应客户端发来了数据
                    char buf[BUFSIZ] = { 0 };
                    int len = read(fds[i].fd, buf, sizeof(buf));

                    syserr(len, "read");

                    if (0 == len) {
                        printf("client closed...\n");

                        //close cfd & clear fdset[i]
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }
                    else if (len > 0) {
                        //读到数据
                        printf("recv data: %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }

        }

    }

    //5. close
    close(lfd);

    return 0;
}