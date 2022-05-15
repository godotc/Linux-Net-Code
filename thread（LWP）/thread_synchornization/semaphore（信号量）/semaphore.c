/*
    int sem_init(&sem,int __pshared, 5);
    - pshared ： 0 用在线程间， 1 用在进程间

    #include <semaphore.h>

       int sem_wait(sem_t *sem);

       int sem_trywait(sem_t *sem);

       int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
        等待多长


    sem_t p_sem;
    sem_t c_sem;

    init(psem,0,8);
    init(psem,0,0);

    producer(){
        sem wait(&p_sem);

        sem_post(&c_sem);
    }

    customer(){
        sem_wait(&c_sem);

        sem_post(&p_sem);
    }

*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

pthread_mutex_t mutex;

//创建两个信号量
sem_t psem;
sem_t csem;


struct Node {
    int num;
    struct Node* next;
};


//头节点
struct Node* head = NULL;


void* f_produce(void* arg) {

    while (1)
    {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);


        //不断创建节点向链表中添加数据
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("Add node, num : %d , tid : %ld \n", newNode->num, pthread_self() % 1000);
        //只要生产了一个，就通知消费者消费


        pthread_mutex_unlock(&mutex);
        sem_post(&csem);    //对消费者信号量+1
        usleep(1000);
    }



    return NULL;
}

void* f_comsume(void* arg) {

    while (1)
    {

        sem_wait(&csem);
        pthread_mutex_lock(&mutex);


        //删除节点
        struct Node* delNode = head;
        head = head->next;
        printf("Del node, num : %d , tid : %ld \n", delNode->num, pthread_self() % 1000);
        free(delNode);


        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(1000);
    }

    return NULL;
}



int main() {


    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);


    //创建5个生产者线程，5个消费者线程
    pthread_t producers[5], comsumers[5];
    for (int i = 0;i < 5;++i) {
        pthread_create(&producers[i], NULL, f_produce, NULL);
        pthread_create(&comsumers[i], NULL, f_comsume, NULL);
    }


    //线程分离
    for (int i = 0;i < 5;++i) {
        pthread_detach(producers[i]);
        pthread_detach(comsumers[i]);
    }


    while (1)
    {
        sleep(10);
    }


    pthread_exit(NULL);


    pthread_mutex_destroy(&mutex);


    return 0;
}