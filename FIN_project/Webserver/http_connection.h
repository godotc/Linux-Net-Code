#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include<stdlib.h>
#include<sys/epoll.h>
#include<stdio.h>
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
#include<string.h>


#include"locker.h"
#include"reterr.h"


class http_connection {
public:

    http_connection() {}
    ~http_connection() {}


    static int m_epollfd;   //所有socket上的事件都被注册到同一个事件中
    static int m_user_count;    //统计用户的数量
    static const int READ_BUFFER_SIZE = 2048;   //读缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 1024;  //写缓冲区的内容

    //HTTP请求的方法,但我们只支持GET
    enum METHOD { GET = 0, POST, PUT, DELETE, TRACE, OPTIONS, CONNECT };

    //解析客户端请求时，主状态机的状态的状态
    enum CHECK_STATE {
        CHECK_STATE_REQUESTLINE = 0,      //当前正在分析请求行
        CHECKSTATE_HEADER,                //头部字段
        CHECK_STATE_CONTENT         //正在分析请求体
    };

    //从状态机的三种可能状态
    enum LINE_STATUS {
        LINE_OK = 0,    //读取到完整行
        LINE_BAD,       //行出错
        LINE_OPEN       //行数据尚且不完整
    };

    //服务器处理HTTP请求的可能结果，常见的
    enum HTTP_CODE {
        NO_REQUEST,     //请求不完整
        GET_REQUSET,    //表示获得一个完整的客户请求
        BAD_REQUSEST,     //表示客户语法错误
        NO_RESOURCE,    //表示服务器没有资源
        FORBIDDEN_REQUEST,  //表示客户对资源没有足够的访问权限
        FILE_REQUEST,      //文件请求，获取文件成功
        INTERNAL_ERROR,     //服务器内部错误
        CLOSE_CONNECTION    //表示客户端以及关闭链接了
    };




    void process();     //处理客户端的请求
    void init(int sockfd, const sockaddr_in& addr); //初始化新接收的链接
    void close_conn();  //关闭链接
    bool read();        //非阻塞的读
    bool write();       //非阻塞的写
    void unmap();       //取消内存映射


private:

    int m_sockfd;       //该 HTTP链接的socket
    sockaddr_in m_address;  //通信的地址
    char m_read_buf[READ_BUFFER_SIZE];  //读缓冲区

    int m_read_idx;    //当前读取的最后一个位置，以及下一次应该读取或储存（行）的首位置索引
    int m_checked_index;    //当前正在分析的字符在读缓冲区的位置
    int m_start_line;   //当前正在解析的行的头位置

    ////////////////////解析请求////////////////////////////
    char* m_url;        //请求目标的文件名
    char* m_version;    //协议版本，只支持http1.1
    METHOD m_method;    //请求方法
    char* m_host;       //主机名
    bool m_linger;      //HTTP请求是否保持链接
    int m_content_length;   //请求体的长度
    char* m_real_file;     //请求的文件(html)的名字
    void* m_file_addr;

    void init();    //初始化链接其余的数据

    //主状态机
    CHECK_STATE m_check_state;  //主状态机当前所处的状态
    
    HTTP_CODE parse_request_line(char* text);   //解析请求行（首行）
    HTTP_CODE parse_headers(char* text);    //解析请求头 (头部里其他的)
    HTTP_CODE parse_content(char* text);     //解析请求体(空行后面的部分)

    //从状态机
    LINE_STATUS parse_line();   //解析每一行
    char* get_line() { return m_read_buf + m_start_line; }      //获取当前行的首地址

    HTTP_CODE do_requset();     //处理请求
    HTTP_CODE process_read();   //解析HTTP请求
    bool process_write(HTTP_CODE read_ret);
    bool write();
    bool add_response(const char* format, ...);

};


#endif