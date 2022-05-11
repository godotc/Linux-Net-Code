/*

    使用多线程 实现卖票案例
    有三个窗口，一共是100张票
*/


#include<stdio.h>
#include<pthread.h>
#include<unistd.h>


int ticket = 1000;

//全局创建互斥量
pthread_mutex_t mutex;


void* sellTicket(void* arg) {


    //卖票
    while (ticket > 0)
    {
        //重复加锁
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);

        if (ticket > 0) {
            //usleep(1000);
            printf("%ld 正在卖第 %d张门票\n",
                pthread_self() % 1000, ticket);
            ticket--;
        }
        else {
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 解锁
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&mutex);

    }

    return NULL;
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);


    //创建三个子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sellTicket, NULL);
    pthread_create(&tid2, NULL, sellTicket, NULL);
    pthread_create(&tid3, NULL, sellTicket, NULL);


    //回收子线程资源,阻塞
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);


    //设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    pthread_exit(NULL); //退出主线程


    //释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;
}