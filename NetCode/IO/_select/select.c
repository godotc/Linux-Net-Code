#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/select.h>

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

    ///////创建一个fd_set的集合，存放需要检测的文件描述符集合
    fd_set readsset, temp;
    FD_ZERO(&readsset);
    FD_SET(lfd, &readsset);
    int maxfd = lfd;

    while (1)
    {
        //调用select系统函数核检测哪些文件描述符有数据
        temp = readsset;
        ret = select(maxfd + 1, &temp, NULL, NULL, NULL);
        syserr(ret, "select");

        if (0 == ret) {
            //意思是没有检测到有数据，但是设置永久阻塞，不可能返回为0
            continue;
        }
        else if (ret > 0) {

            //检测到有文件描述符发生了改变
            if (FD_ISSET(lfd, &temp))
            {
                //表示有新的客户端链接进来了
                struct sockaddr_in client_addr;
                int len = sizeof(client_addr);

                //4. accept
                int cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);

                //将新的文件描述符加入到集合中
                FD_SET(cfd, &readsset);

                //更新右区间（最大的文件描述符）
                maxfd = maxfd > cfd ? maxfd : cfd;
            }

            for (int i = lfd + 1;i <= maxfd; i++) {
                if (FD_ISSET(i, &temp)) {

                    //说明这个文件描述符的对应客户端发来了数据
                    char buf[BUFSIZ] = { 0 };
                    int len = read(i, buf, sizeof(buf));

                    syserr(len, "read");

                    if (0 == len) {
                        printf("client closed...\n");

                        //close cfd & clear fdset[i]
                        close(i);
                        FD_CLR(i, &readsset);                 
                    }
                    else if (len > 0) {
                        //读到数据
                        printf("recv data: %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }
            
        }
        
    }

    //5. close
    close(lfd);

    return 0;
}