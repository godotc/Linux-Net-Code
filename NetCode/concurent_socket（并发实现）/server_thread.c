#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

struct sockInfo {
    int fd; //文件描述符
    pthread_t tid; //线程号
    struct sockaddr_in addr;
};

struct sockInfo sockInfos[128];  //支持128个链接同时连接


void serr(char* str) {
    perror("str");
    exit(-1);
}

void* working(void* arg) {
    //子线程与客户端通信

    //转换传入的void*指针
    struct sockInfo* pInfo = (struct sockInfo*)arg;

    //获取客户端的信息
    char client_IP[16];
    inet_ntop(AF_INET, &pInfo->addr.sin_addr, client_IP, sizeof(client_IP));
    unsigned short client_Port = ntohs(pInfo->addr.sin_port);
    //
    printf("client IP : %s , Port: %d\n", client_IP, client_Port);

    //接收客户端发来的数据
    char buf[BUFSIZ];
    while (1)
    {
        int len = read(pInfo->fd, buf, BUFSIZ);

        if (-1 == len)serr("read");

        else if (len > 0) printf("receive client: %s\n", buf);

        else if (0 == len)
        {
            printf("client closed...\n");
            break;
        }

        write(pInfo->fd, buf, strlen(buf) + 1);
    }

    close(pInfo->fd);
    return NULL;
}

int main() {
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == lfd)serr("socket");


    struct sockaddr_in saddr;
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9527);

    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (-1 == ret)serr("bind");

    ret = listen(lfd, 128);
    if (-1 == ret)serr("listen");

    //初始化数据
    int max = sizeof(sockInfos) / sizeof(sockInfos[0]);
    for (int i = 0;i < max;i++) {
        bzero(&sockInfos[i], sizeof(sockInfos[i]));
        sockInfos[i].fd = -1;
        sockInfos[i].tid = -1;
    }

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        int cfd = accept(lfd, (struct sockaddr*)&client_addr, &len);
        if (-1 == cfd)serr("accept");

        //创建线程
        struct sockInfo* pInfo;

        for (int i = 0;i < max;i++) {
            //从这个数组中找到能用的sockInfo元素
            if (-1 == sockInfos[i].fd) {
                pInfo = &sockInfos[i];
                break;
            }
            //如果找完没有剩余就等待后 -1
            if (i == max - 1) {
                sleep(1);
                i--;
            }
        }

        pInfo->fd = cfd;
        memcpy(&pInfo->addr, &client_addr, len);

        //创建子线程
        pthread_create(&pInfo->tid, NULL, working, pInfo);

        //回收资源，不能用ptr_join阻塞
        pthread_detach(pInfo->tid);

    }

    close(lfd);

    return 0;
}