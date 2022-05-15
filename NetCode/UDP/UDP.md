# 1. UDP通信

     #include <sys/types.h>
       #include <sys/socket.h>

       ssize_t send(int sockfd, const void *buf, size_t len, int flags);
---
       ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
       
            - flags : 一般为0 
            - dest_addr: 通信另一端的地址信息
            - addr_len
        返回值：成功，返回发送的数量，失败 -1

---
       ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);


       ssize_t recv(int sockfd, void *buf, size_t len, int flags);
---
       ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);

            - flags: 0 
            - src_addr : 源 从哪接收，可做传出参数保存另一端IP地址，不需要可以指定为NULL；

---

       ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);

## 1.2 广播

1. 只能在局域网内用
2. 一个地址 192.168.10.20 前三个8进制为 网络ID，最后8位为主机ID（为0为子网IP）
3. 客户端要绑定服务器广播使用的port，才可以收到广播消息
4. 通过setscckopt 设置广播属性

       #include <sys/types.h>          /* See NOTES */
       #include <sys/socket.h>

       int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
       int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);

       - level : SOL_SOCKEKT
       - optname: SO_BROADCAST
       - optval: int value 为1表示允许广播
       - optlen : optval 的大小

---

## 1.3 组播（多播）
1. 多对多，可以选定特定的用户
2. 组播既可以用于局域网，也可以用于广域网
3. 客户端要加入多播组，才能接收到数据

- IP 多播地址 从224.0.0.0 ~ 239.255.255.255

<img src = "../pic/组播.png" >

       int setsockopt(int sockfd, int level,int optname, cont void*optval, socklen);

       //服务端设置要多播的信息，外出接口
       - level: IPPROTO_IO
       - optname: IP_MULTICAST_IF
       - optval: struct in_addr

       struct ip_mrqn{

              struct in_addr imr_multiaddr;      //组播的IP地址
              struct in_addr imr_address;        //本地某一网络设备接口的IP地址

              int imr_ifindex;            //网卡编号
       }

       //客户端要加入多播组
       - level: IPPROTO_IP
       - optname:  IP_ADD_MEMBERSHIP
       - optval : struct ip_mreq