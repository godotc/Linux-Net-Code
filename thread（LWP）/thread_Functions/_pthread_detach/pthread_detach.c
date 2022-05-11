/*
#include <pthread.h>

        int pthread_detach(pthread_t thread);
        当一个分离线程结束时，他的资源自动 release back;

        - 不能再去 链接 一个已经 detach 分离的线程，会报错

        - retval:   s 0  F errnum

Compileand link with - pthread.

*/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL;
}


int main() {

    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* errstr = strerror(ret);
        printf("create: %s\n", errstr);
    }

    //输出主线程与子线程的id
    printf("tid: %ld, main thread id : %ld \n", tid, pthread_self());


    //设置子线程分离
    ret = pthread_detach(tid);
    if (ret != 0) {
        char* errstr = strerror(ret);
        printf("detach: %s\n", errstr);
    }

    //分离后，对子线程链接 错误产生
    ret = pthread_join(tid, NULL);
    if (ret != 0) {
        char* errstr = strerror(ret);
        printf("join: %s\n", errstr);
    }

    pthread_exit(NULL);


    return 0;

}