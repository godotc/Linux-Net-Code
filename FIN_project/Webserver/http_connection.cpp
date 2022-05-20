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

    //event.events = EPOLLIN | EPOLLRDHUP/*挂起*/;
    event.events = EPOLLIN | EPOLLRDHUP/*挂起                                                */|EPOLLET/*边缘触发*/;

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

    init();
}

//初始化读取与分析的位置表示符
void http_connection::init() {
    m_check_state = CHECK_STATE_REQUESTLINE;    //初始化状态为解析请求首行
    m_checked_index = 0;    //当前以及解析的字符
    m_start_line = 0;   //当前的行
    m_read_idx = 0;     //从socket读取数据的index
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

//循环读取客户数据，知道无数据可读或对方关闭链接
bool http_connection::read()
{
    if (m_read_idx >= READ_BUFFER_SIZE) {
        return false;
    }

    //读取到的字节
    int bytes_read = 0;
    while (1)
    {
        bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE, 0);
        if (bytes_read == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //没有数据了
                break;
            }
            return false;
        }
        else if (bytes_read == 0)
        {
            //对方关闭链接
            return false;
        }
        m_read_idx += bytes_read;   //让m_read_idx 等于 读到request的末尾
    }
    printf("读取到了数据: % s\n"
        , m_read_buf);
    return true;
}

//由线程池中的工作线程调用，处理HTTP请求的入口函数
void http_connection::process()
{
    // 解析HTTP请求
    HTTP_CODE read_ret = process_read();
    if (NO_REQUEST == read_ret) {
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        return;
    }

    // 生成响应

}

//（主状态机）解析HTTP请求
http_connection::HTTP_CODE http_connection::process_read(char* text) {

    //定义初始状态
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;

    char* text = 0;

    while (((line_status = parse_line()) == LINE_OK) ||
        (m_check_state == CHECK_STATE_CONTENT) && line_status == LINE_OK)
    {
        //解析完成了一行完整的数据 或者 解析到了请求体

        //获取一行数据
        text = get_line();

        m_start_line = m_checked_index;
        printf("got 1 http line: %s\n", text);

        switch (m_check_state)
        {
        case CHECK_STATE_REQUESTLINE:
        {
            //解析请求行
            ret = parse_line(text);
            if (BAD_REQUSEST == ret) {
                return BAD_REQUSEST;
            }
            break;
        }
        case CHECKSTATE_HEADER:
        {
            //解析请求头
            ret = parse_headers(text);
            if (BAD_REQUSEST == ret) {
                return BAD_REQUSEST
            }
            else if (ret == GET_REQUSET) {
                //成功获取了完整请求头
                return do_request();
            }
            break;
        }
        case CHECK_STATE_CONTENT:
        {
            //解析请求体
            ret = parse_content();
            if (GET_REQUSET == ret) {
                return do_requset();
            }
            line_status = LINE_OPEN;
            break;
        }

        default:
            return INTERNAL_ERROR;
            break;
        }
    }
    return NO_REQUEST;
}


//解析请求行
http_connection::HTTP_CODE http_connection::process_request_line(char* text) {
    
}

//解析请求头
http_connection::HTTP_CODE http_connection::parse_headers(char* text);

//解析请求体
http_connection::HTTP_CODE http_connection::parse_content(char* text);



//解析每一行，以及\r\n判断
http_connection::LINE_STATUS http_connection::parse_line(char* text)
{
    char temp;
    for (;m_checked_index < m_read_idx;++m_checked_index)
    {
        temp = m_read_buf[m_checked_index];
        if ('\r' == temp) {
            if (m_read_idx == (m_checked_index + 1))
            {
                //读到了缓冲区里数据的末尾
                return LINE_OPEN;
            }
            else if ('\n' == m_read_buf[m_checked_index + 1])
            {
                m_read_buf[m_checked_index++] = '\0';   //标记当前字节为字符串结束符，并++移到下一位
                m_read_buf[m_checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if ('\n' == temp) {
            if ((m_checked_index > 1) && (m_read_buf[m_checked_index - 1] == '\r'))
            {
                //与上一次缓存区的\n构成换行
                m_read_buf[m_checked_index - 1] = '\0';
                m_read_buf[m_checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        return LINE_OPEN;
    }
    return LINE_OK;
}



//根据主状态机的状态进行不同的处理
http_connection::HTTP_CODE http_connection::do_requset() {

}


bool http_connection::write() {
    printf("一次性写完数据\n");
    return true;
}






