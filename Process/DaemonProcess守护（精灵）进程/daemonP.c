/*
写一个守护进程，每隔两秒获取时间写入磁盘
*/


#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/time.h>
#include<signal.h>
#include<time.h>
#include<string.h>

void work(int num) {
    //捕捉到信号以后获取时间，写入磁盘文件
    time_t m_time = time(NULL);
    //将秒数转换为时间
    struct tm* loc = localtime(&m_time);

    //char buf[1024];
    //sprintf(buf, "%d-%d-%d %d:%d:%d\n", loc->tm_year + 1900, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
    //printf("%s\n", buf);


    char* str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, str, strlen(str));
    close(fd);
}

int main() {

    //1.创建子进程，退出父进程
    pid_t pid = fork();
    if (pid > 0) exit(0);

    //2.将子进程重新创建一个回话
    setsid();

    //3.设置掩码
    umask(022);

    //4.更改工作目录
    chdir("~/lesson/process进程/DaemonProcess守护（精灵）进程/");

    //5.关闭、重定向文件描述符
    int fd = open("/dev/null", O_RDWR);     //这个地方会忽略所有输出
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    //6. 业务逻辑

    //捕捉 SIGALARM 信号
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    //创建定时器
    struct itimerval val;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &val, NULL);

    //不让进程结束
    while (1)
    {
        sleep(10);
    }


    return 0;
}