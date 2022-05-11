/*

#include <pthread.h>

    int pthread_cancel(pthread_t thread);

    - 取消进程（让线程终止）‘
      但是并不是立马终止，而是当线程执行到一个取消点
        - 取消点： 系统规定的一些调用，粗略理解为从用户区到系统区的位置

Compileand link with - pthread.


*/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    for (int i = 0;i < 10;++i) {
        printf("child: %d\n", i);
    }
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


    //取消线程  
    ret = pthread_cancel(tid);
    if (ret != 0) {
        char* errstr = strerror(ret);
        printf("cancel: %s\n", errstr);
    }


    for (int i = 0;i < 5;++i) {
        printf("father: %d\n", i);
    }

    pthread_exit(NULL);


    return 0;

}