#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex;


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

            head = head->next;
            printf("Del node, num : %d , tid : %ld \n", delNode->num, pthread_self() % 1000);
            free(delNode);

            pthread_mutex_unlock(&mutex);

            usleep(1000);

        }
        else {
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}



int main() {


    pthread_mutex_init(&mutex, NULL);


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