/*


#include <pthread.h>

        int pthread_join(pthread_t thread, void** retval);

        - 功能：和一个已终止的线程进行链接（阻塞的）
                回收子线程的资源
                一般在主线程中使用
        - void** retavl 接受子线程退出的返回值

        - 返回值 ： 成功 0 非0 失败错误号

Compileand link with - pthread.

*/



#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>


void* callback(void* strcasecmp) {
    printf("child thread id : %ld \n", pthread_self());
    sleep(3);
    static int value = 10;     //局部变量，结束后被清空，传地址乱码

    pthread_exit((void*)&value);

    //return  NULL;   //等同于 pthread_exit(NULL)
}

int main() {

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0)
    {
        char* str = strerror(ret);
        printf("cret:%s\n", str);
    }

    for (int i = 0;i < 50;i++) {
        printf("%d\n", i);
    }

    printf("tid: %ld , main thread id : %ld \n", tid, pthread_self());


    //主线程调用pthread_join()回收子线程资源
    // 阻塞等待子线程结束
    int* thread_retval;

    ret = pthread_join(tid, (void**)&thread_retval);

    if (ret != 0)
    {
        char* errstr = strerror(ret);
        printf("err:%s\n", errstr);
    }
    printf("子线程资源回收成功\n");

    printf("thread retrurn val: %d\n", *thread_retval);

    pthread_exit(NULL);


    //退出后无法执行后续代码
    printf("main thread exit\n");

    return 0;
}