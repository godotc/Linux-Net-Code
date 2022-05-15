# 1. I/O多路复用（I/O多路链接）
- 使用多线程、多进程开销太大，根源是 读写的阻塞
- 可以使程序同时监听多个文件描述符,提高程序性能，通过I/O多路复用  
主要有select、poll、epoll

# 2. select
- 主旨思想：
1. 构造一个关于文件描述符的列表（监听的sockd fd）加入其中
2. 调用一个系统函数，监听其中的fd，直到这些描述符中的一个或多个进行I/O操作时，才返回
    - 这个函数是阻塞的
    - 对fd的检测是由内核完成的
3. 在返回时，会告诉有哪些（多少）描述符需要I/O操作

        #include <sys/time.h>
        #include <sys/types.h>
        #include <unistd.h>

        int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

        - 参数:
            - nfds :
                - 委托内核检测的最大fd加1
                - 内核遍历的右区间
            - readfds: 要检测的fd集合set，委托内核检测哪些文件描述符的属
                - 一般是检测读操作
                - 对应是对方发送过来的数据，应为读是被动的接收数据，检测的就是读缓冲区
                - 是一个传入传出参数
            - writefds : 
                - 有write写属性的fd的集合
                - 委托内核检测写缓冲区是否满了，是否能写数据
            - exeptfds： 检测发生异常的文件描述符集合
            - timeval* timeout ：设置的超时时间
                - NULL ： 永久阻塞，知道检测到了文件描述符有变化
                - tv_sec = 0 tv_usec =0 不阻塞
                - >0 阻塞对应的时间      
        - 返回值 ： -1 失败
                     n 检测的集合中有n个文件描述符发生了变化  

        //sizeof(fd)==128 使用下面改变API进行位操作改变fd的属性
        void FD_CLR(int fd, fd_set *set);
        - clear 将对应的标志位设置为0

        int  FD_ISSET(int fd, fd_set *set);
        - 判断对应标志位是否为1，返回对应标志位的值

        void FD_SET(int fd, fd_set *set);
        - 设置对应的为1

        void FD_ZERO(fd_set *set);
        - 清空 set中的值

        #include <sys/select.h>

        int pselect(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timespec *timeout, const sigset_t *sigmask);

>总结
- 每次将fds从用户态拷到内核态，然后遍历O(n)
- 拷贝回来，再次n次遍历，开销太大了
- select支持的文件描述符太小了，默认1024
- fds集合不能重用


# 3. poll
- 实现原理与select类似


    struct pollfd {
               int   fd;         /* file descriptor */
               short events;     /* requested events */检测时事件
               short revents;    /* returned events */返回事件
           };

        
        #include <poll.h>

       int poll(struct pollfd *fds, nfds_t nfds, int timeout);
        - 没有1024的限制
        - nfds_t nfds: 第一个参数数组中最后一个有效元素的下标+1
        - timeout： 阻塞时长 0 表示不阻塞 
                            -1 表示阻塞：有变化才解除返回

        返回： -1 失败
                n(>0)  n个发生变化

       #define _GNU_SOURCE 
        #include <signal.h>
       #include <poll.h>

       int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *tmo_p, const sigset_t*sigmask);

>总结
- 对select的改进，扩大的max fd 的abountant
- 同样需要遍历，在用户与内核之间拷贝

# 4. epoll
 
- 调用epoll_create直接创建一个实例在内核，返回一个fd来操纵实例

- 这个数据结构中有两个比较重要的数据:
    - __struct rb_root rbr;__       需要检测的fd集合，是一个红黑树
    - <b>strucht list_head rdlist;   </b> 改动的fd的就绪列表，是一个双向链表

    #include <sys/epoll.h>

       int epoll_create(int size);
        - size: 目前没有意义了must greater than 0
        - return:  -1 失败
                   >0 文件描述符 epfd

       int epoll_create1(int flags);

       int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
       作用： 对epoll实例进行管理
            - op : 操作 
                - EPOL_CTL_ADD/MOD/DEL 
            - fd: 检测fd
            - event: 检测什么事情
            - event:
        struct epoll event{
            uint32_t events;   //epoll events;
            epoll_data_t data; //user data variable
        };
        - events: 
            常见epoll事件： EPOLLIN/OUT/ERR
                            EPOLLET 边缘触发
        - data:  

        typedef union epoll_data{
            void *ptr
            int fd;
            uint32_t u32;
            u64;
        }epoll_data_t;    



       int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
            - events 传出参数，保存变化了的文件描述符
            - maxevents : 最大fd
            - timeout ： 0不阻塞 -1 阻塞
        返回值：成功，返回发送变化的fd个数

       int epoll_pwait(int epfd, struct epoll_event *events,int maxevents, int timeout,const sigset_t *sigmask);

## epoll的两种工作模式

>LT模式（level-triggered) （水平触发）
- 缺省的工作模式，即不去指定就是LT，surppot __block__ & __non block__ socket

- 只要fd里面有数据，如果你不做任何操作，内核就会通知你的  
            
        >假设委托内核检测读事件->检测fd的读缓冲区
        
        1.用户不读数据，数据一直待缓冲区，epoll会一直通知
        2.用户只读了一部分数，epoll会通知
        3. 缓冲区中的数据读完了，不通知

<br>

>ET (Edge-Triggered)(边沿触发)
- 高速状态下运行，只支持 non-block socket

- 当fd 从 未就绪到就绪，内核会通过epoll通知你，然后他会假设你已知道，不会发这个fd的就绪通知,直到在对这个fd做IO操作（only once）
- 很大程度减少了epoll此处，提高IO效率

        >假设委托内核检测读事件->检测fd的读缓冲区
        
        1.读缓冲区有数据->epoll会给用户通知
            a. 用户不读数据，数据一直在缓冲区中，epoll下次不通知
            b. 用户读了一部分， 不通知
            c. 缓冲区读完了，不通知
        2.再次进行I/O->通知
