/*


 #include <pthread.h>

        void pthread_exit(void *retval);

        终止一个线程，在哪个线程中调用就终止哪个线程

        void*retval: 需要传递一个指针作为返回值，可以在pthread_join()中获取到


        Compile and link with -pthread.

        int pthread_equal(t1,t2)
        不同操作系统phread_t实现类型不同，有的是长整型有的是结构体
        > 用函数来比较线程id是否相等
        成功非0值，失败0

*/

#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>


void* callback(void* strcasecmp) {
    printf("child thread id : %ld \n", pthread_self());
    return  NULL;   //等同于 pthread_exit(NULL)
}

int main() {

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* str = strerror(ret);
        printf("cret:%s\n", str);
    }

    for (int i = 0;i < 50;i++) {
        printf("%d\n", i);
    }

    printf("tid: %ld , main thread id : %ld \n", tid, pthread_self());


    //主线程退出，不会影响其他线程
    pthread_exit(NULL);

    //退出后无法执行后续代码
    printf("main thread exit\n");

    return 0;
}