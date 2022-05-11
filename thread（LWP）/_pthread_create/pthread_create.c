/*

#include <pthread.h>

    一般 main函数所在的线程称为主线程（main线程）；
    程序中默认只有一个进程，fork（）后，两个线程
    此时 pthread_create（）调用会产生两个子线程

    int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg);

        功能： 创建一个子线程

            -pthread_t* thread: 传出参数，线程创建成功后，子线程id 写入该地址中
            -pthread_attr_t* attr: 设置线程的属性，一般NULL默认
            -void* (*start_routine)(void*): 函数指针，子线程干的
            - arg ： 给函数指针传参

        返回值 成功 0 
                失败 错误号 --- 区别于之前 ， 无法用 perror 输出
                        获取方法： char* strerror(int errnum);


Compileand link with - pthread.

*/

#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

void* callback(void* arg) {

    printf("Child thread.. value = %d\n", *(int*)arg);

    return NULL;
}

int main() {

    pthread_t tid;

    int number = 10;
    //创建一个子线程
    int ret = pthread_create(&tid, NULL, callback, (void*)&number);

    if (ret != 0) {
        char* str = strerror(ret);
        printf("error: %s\n", str);
    }

    sleep(1);
    for (int i = 0;i < 5;i++) {
        printf("%d\n", i);
    }

    return 0;
}