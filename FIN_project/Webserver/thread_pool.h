#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include<pthread.h>
#include<unistd.h>
#include<list>
#include<locker.h>

//线程池类，定义成模板类，为了代码的复用，T就是任务类
template<class T>
class Thread_poll
{
public:
    Thread_poll(/* args */);
    ~Thread_poll();
private:
    
    int m_thread_number;    //线程的数量   
    pthread_t* m_threads;   //线程池数组,大小为m_thread_number
    
    int m_max_requests;     //请求队列中最多允许的，等待处理的请求数量
    std::list<T*> m_workQueue;  //请求队列

    Locker m_queueLocker;   //互斥锁

    Semaphore m_queseStat   //用信号量来判断是否有任务要处理
};






#endif