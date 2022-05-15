#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex;
//创建条件变量
pthread_cond_t condi;


struct Node {
    int num;
    struct Node* next;
};


//头节点
struct Node* head = NULL;


void* f_produce(void* arg) {

    while (1)
    {
        pthread_mutex_lock(&mutex);

        //不断创建节点向链表中添加数据
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("Add node, num : %d , tid : %ld \n", newNode->num, pthread_self() % 1000);
        //只要生产了一个，就通知消费者消费
        pthread_cond_signal(&condi);

        pthread_mutex_unlock(&mutex);

        usleep(1000);
    }



    return NULL;
}

void* f_comsume(void* arg) {

    while (1)
    {
        pthread_mutex_lock(&mutex);

        //删除节点
        struct Node* delNode = head;

        if (head) {
            //有数据
            head = head->next;
            printf("Del node, num : %d , tid : %ld \n", delNode->num, pthread_self() % 1000);
            free(delNode);

            pthread_mutex_unlock(&mutex);

            usleep(1000);

        }
        else {
            //没有数据，需要等待,wait调用阻塞时，会解锁，不阻塞的时候继续执行会加锁 
            pthread_cond_wait(&condi, &mutex);
            pthread_mutex_unlock(&mutex);
        }

    }

    return NULL;
}



int main() {


    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condi, NULL);


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
    pthread_cond_destroy(&condi);

    return 0;
}