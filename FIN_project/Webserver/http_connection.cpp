#include"http_connection.h"


int http_connection::m_epollfd = -1;
int http_connection::m_user_count = 0;


void setnonblocking(int fd)
{
    int old_flag = fcntl(fd, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flag);
}

//先epoll中添加需要监听的fd
void addfd(int epollfd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    //epoll让内核检测什么事件，不写EPOLLET为默认水平触发
    event.events = EPOLLIN | EPOLLRDHUP/*挂起*/;

    // Oneshot: 一个线程在处理fd的数据时，又有新数据到达，可能会唤醒另一个线程来处理，使用oneshot判断一个文件描述符只能由一个线程处理
    if (one_shot) {
        event.events | EPOLLONESHOT;
    }

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

    //设施文件描述符非阻塞
    setnonblocking(fd);
}

//从epoll中删除文件描述符
void removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
    close(fd);
}

//修改文件描述符，重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
void modfd(int epollfd, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

//初始化链接
void http_connection::init(int sockfd, const sockaddr_in& addr)
{
    //将http_conn即users数组中的每一个新链接的sock和
    m_sockfd = sockfd;
    m_address = addr;

    //设置 client-http_conn 端口复用
    int reuse = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

    //添加到epoll对象epollfd中
    addfd(m_epollfd, m_sockfd, true);
    m_user_count++;
}

//关闭链接
void http_connection::close_conn()
{
    if (m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}


bool http_connection::read() {
    printf("一次性读完数据\n");
    return true;
}

bool http_connection::write() {
    printf("一次性写完数据\n");
    return true;
}

//由线程池中的工作线程调用，处理HTTP请求的入口函数
void http_connection::process()
{
    // 解析HTTP请求

    printf("parse request,creat response\n");

    // 生成响应

}




