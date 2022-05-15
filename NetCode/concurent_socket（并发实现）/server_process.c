#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<wait.h>

void err(char* str) {
    perror("str");
    exit(-1);
}

void recycleChild(int arg) {
    while (1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1) {
            //所有的子进程都回收完了
            break;
        }
        else if (ret == 0)
        {
            //还有子进程或者
            break;
        }
        else if (ret > 0)
        {
            //被回收了
            printf("进程号  ：%d ，        被回收了\n",ret);
        }
    }
}

int main() {

    //注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;
    sigaction(SIGCHLD, &act, NULL);

    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == lfd) err("socket");

    //绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9527);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (-1 == ret)err("bind");

    //监听
    ret = listen(lfd, 128);
    if (-1 == ret)err("listen");



    //不断循环等待客户端链接
    while (1)
    {
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);

        //接受链接
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
        if (-1 == cfd)err("accept");

        //每一个链接进来，就创建一个子进程处理客户端链接
        pid_t pid = fork();

        if (-1 == pid)err("fork");
        else if (0 == pid)
        {
            //子进程  获取客户端的信息
            char cliIp[16];
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIp, sizeof(cliIp));
            unsigned short cliPort = ntohs(cliaddr.sin_port);
            printf("client IP : %s , Port : %d \n", cliIp, cliPort);

            //子进程接受数据
            char buf[BUFSIZ] = { 0 };
            while (1) {
                int len = read(cfd, &buf, sizeof(buf));
                if (-1 == len) err("read");
                else if (len > 0) printf("receive client data: %s \n", buf);
                else {
                    printf("client closed...\n");
                    break;
                }

                write(cfd, buf, strlen(buf));
            }
            close(cfd);
            exit(0);    //退出当前子进程
        }
    }


    close(lfd);

    return 0;
}