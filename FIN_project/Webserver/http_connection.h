#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H


#include<sys/epoll.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/mman.h>
#include<stdarg.h>
#include<errno.h>
#include<sys/uio.h>

#include"locker.h"
#include"reterr.h"


class http_connection {
public:

    http_connection() { }
    ~http_connection() {}


    static int m_epollfd;   //所有socket上的事件都被注册到同一个事件中
    static int m_user_count;    //统计用户的数量

    void process();     //处理客户端的请求
    void init(int sockfd, const sockaddr_in& addr); //初始化新接收的链接
    void close_conn();  //关闭链接
    bool read();        //非阻塞的读
    bool write();       //非阻塞的写

private:

    int m_sockfd;       //该 HTTP链接的socket
    sockaddr_in m_address;  //通信的地址

};


#endif