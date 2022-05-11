
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>


void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL;
}


int main() {

    //创建一个线程属性变量
    pthread_attr_t attr;

    //初始化变量属性
    pthread_attr_init(&attr);

    //设置属性
    int ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


    pthread_t tid;

    //第二个参数传入属性
    ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0) {
        char* errstr = strerror(ret);
        printf("create: %s\n", errstr);
    }


    //获取线程栈的大小
    size_t stsize;
    pthread_attr_getstacksize(&attr, &stsize);
    printf("thread stack size: %ld\n", stsize);



    //输出主线程与子线程的id
    printf("tid: %ld, main thread id : %ld \n", tid, pthread_self());


    //释放线程属性资源
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);


    return 0;

}