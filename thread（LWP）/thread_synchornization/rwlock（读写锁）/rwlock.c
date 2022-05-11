/*
    创建8个线程，操作一个全局变量
    3个不定时写，其余不定时读
*/

#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

//创建一个共享数据
int num = 1;


//pthread_mutex_t mutex; 同时只能有一个线程能读>用rwlock优化
pthread_rwlock_t rwlock;

void* f_Write(void* arg) {
    while (1)
    {
        //pthread_mutex_lock(&mutex);
        pthread_rwlock_wrlock(&rwlock);

        num++;
        printf("++ write, tid : %ld , num : %d \n", pthread_self() % 1000, num);

        //pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);

        usleep(100);
    }
    return  NULL;
}

void* f_Read(void* arg) {
    while (1)
    {
        //pthread_mutex_lock(&mutex);
        pthread_rwlock_rdlock(&rwlock);

        printf("== read , tid : %ld , num : %d \n", pthread_self() % 1000, num);

        //pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);

        usleep(100);
    }
    return NULL;
}



int main() {

    //pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);


    //初始化读写锁



    //创建3个写线程5个读线程
    pthread_t wtids[3], rtids[5];
    for (int i = 0;i < 3;i++) {
        pthread_create(&wtids[i], NULL, f_Write, NULL);
    }
    for (int i = 0;i < 5;i++) {
        pthread_create(&rtids[i], NULL, f_Read, NULL);
    }


    //设置线程分离
    for (int i = 0;i < 3;i++) {
        pthread_detach(wtids[i]);
    }
    for (int i = 0;i < 5;i++) {
        pthread_detach(rtids[i]);
    }


    pthread_exit(NULL);


    //pthread_mutex_destroy(&mutex);
    pthread_rwlock_init(&rwlock, NULL);

    return  0;
}