#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<signal.h>
#include<cstdio>

#include"locker.h"
#include"thread_pool.h"
#include"http_connection.h"
#include"reterr.h"

#define MAX_FD 65535  //最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000 //监听的最大事件数量





//添加信号捕捉
void addsig(int sig, void(handler)(int));

//添加文件描述符到epoll中
extern void addfd(int epollfd, int fd, bool one_shot);

//从epoll中删除文件描述符
extern void removefd(int epollfd, int fd);

////修改文件描述符，重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
extern void modfd(int epollfd, int fd, int ev);


int main(int argc, char** argv) {

    if (argc <= 1) {
        std::printf("按照如下格式运行:./%s port_number \n",basename(argv[0]));
        exit(-1);
    }

    //获取端口号
    int Port = atoi(argv[1]);

    //对 SIGPIE 信号进行处理    
    /*
    当服务器close一个连接时，若client端接着发数据。根据TCP协议的规定，会收到一个RST响应，client再往这个服务器发送数据时，系统会发出一个SIGPIPE信号给进程，告诉进程这个连接已经断开了，不要再写了。*/
    addsig(SIGPIPE, SIG_IGN);



    //创建线程池，初始化线程池
    Thread_pool<http_connection>* pool = nullptr;
    try {
        pool = new Thread_pool<http_connection>;
    }
    catch (...) {
        exit(-1);
    }

    //创建一个数组用于保存所有的客户端信息
    http_connection* users = new http_connection[MAX_FD];


    // socket
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    reterr(listenfd, "socket");

    //在绑定前设置端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

    // bind
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(Port);
    int ret = bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    reterr(ret, "bind");


    // listen
    ret = listen(listenfd, 5);
    reterr(ret, "listen");

    //创建epoll对象——事件数组——添加文件描述符
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(1);

    //将监听的文件描述符添加到epoll对象中
    addfd(epollfd, listenfd, false);
    http_connection::m_epollfd = epollfd;

    while (1)
    {
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        //判断失败情况，失败小于0并且不是，EINTR是读到为空中断，num是传回的变动的事件描述符数量
        if ((num < 0) && (errno != EINTR))
        {
            printf("epoll failure\n");
            break;
        }

        //循环遍历传入的事件数组
        for (int i = 0;i < num;i++)
        {

            int sockfd = events[i].data.fd;

            if (sockfd == listenfd)
            {
                // accept 有新的客户端链接进来
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);

                // accept if client connectting
                int connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
                reterr(connfd, "accept", false);

                if (http_connection::m_user_count >= MAX_FD) {
                    // 目前链接数满了
                    // 给客户端写一个信息，服务器正忙
                    close(connfd);
                    continue;
                }
                //将新的客户的数据初始化，放到数组中
                users[connfd].init(connfd, client_addr);
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //对方异常断开或错误等事件
                users[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLIN)
            {
                // read
                if (users[sockfd].read()) {
                    //一次性发所有数据读完
                    pool->append(users + sockfd);   //数组收地址+sockfd号 ->偏移地址
                }
                else {
                    //发生了错误
                    users[sockfd].close_conn();
                }
            }
            else if (events[i].events & EPOLLOUT)
            {
                // write
                if (!users[sockfd].write()) {
                    //一次性写完了所有数据
                    users[sockfd].close_conn();
                }

            }
        }
    }

    close(epollfd);
    close(listenfd);
    delete[] users;
    delete pool;

    return 0;
}









void addsig(int sig, void(handler)(int)) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;

    //设置临时阻塞信号集合
    sigfillset(&sa.sa_mask);
    //注册信号
    sigaction(sig, &sa, nullptr);
}