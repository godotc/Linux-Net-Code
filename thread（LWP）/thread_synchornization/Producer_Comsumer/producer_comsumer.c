#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>


struct Node {
    int num;
    struct Node* next;
};


//头节点
struct Node* head = NULL;


void* f_produce(void* arg) {

    while (1)
    {
        //不断创建节点向链表中添加数据
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("Add node, num : %d , tid : %ld \n", newNode->num, pthread_self() % 1000);
        //continue
    }



    return NULL;
}

void* f_comsume(void* arg) {

    while (1)
    {
        //删除节点
        struct Node* delNode = head;
        head = head->next;
        free(delNode);
    }

    return NULL;
}



int main() {


    //创建5个生产者线程，5个消费者线程
    pthread_t producers[5], comsumers[5];
    for (int i = 0;i < 5;++i) {
        pthread_create(&producers[i], NULL, f_produce, NULL);
        pthread_create(&comsumers[i], NULL, f_comsume, NULL);
    }


    //线程分离
    for (int i = 0;i < 5;++i) {
        thread_detach(&producers[i], NULL);
        thread_detach(&comsumers[i], NULL);
    }


    // while (1)
    // {
    //     sleep(10);
    // }


    pthread_exit(NULL);

    return 0;
}