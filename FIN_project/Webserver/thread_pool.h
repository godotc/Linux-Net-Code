#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include<pthread.h>
#include<unistd.h>
#include<list>
#include<locker.h>
#include<iostream>

//线程池类，定义成模板类，为了代码的复用，T就是任务类
template<class T>
class Thread_pool
{
public:

    Thread_pool(int thread_Num = 8, int max_Request = 10000);


    ~Thread_pool();

    bool append(T* request);

private:
    static void* worker(void* arg);
    void run();

private:
    //线程的数量 
    int m_thread_number;

    //线程池数组,大小为m_thread_number
    pthread_t* m_threads;

    //请求队列中最多允许的，等待处理的请求数量
    int m_max_requests;

    //请求队列
    std::list<T*> m_workQueue;

    //互斥锁
    Locker m_queueLocker;

    //用信号量来判断是否有任务要处理
    Semaphore m_queseStat;

    //是否结束进程
    bool m_stop;
};


template<typename T>
Thread_pool<T>::Thread_pool(int thread_Num = 8, int max_Request = 10000) :
    m_thread_number(thread_Num), m_max_requests(max_Request), m_stop(false), m_threads(nullptr) {

    if ((m_thread_number <= 0) || (max_Request <= 0)) {
        throw std::exception();
    }

    m_threads = new pthread_t[m_thread_number];
    if (!m_threads) {
        throw std::exception();
    }

    //创建 thread_number个线程，并将他们设置为detach
    for (int i = 0;i < m_thread_number;++i) {
        std::cout << "created the" << i << "th thread" << std::endl;

        //将this传入静态woker中
        if (0 != pthread_create(m_threads + i, nullptr, worker, this)) {
            delete[]m_threads;
            throw std::exception();
        }

        if (pthread_detach(m_thread[i])) {
            delete[]  m_threads;
            throw std::exception();
        }
    }

};

template<typename T>
Thread_pool<T>::~Thread_pool() {
    delete[]m_threads;
    m_stop = true;
}

template<typename T>
bool Thread_pool<T>::append(T* request) {

    m_queueLocker.lock();
    if (m_workQueue.size() > m_max_requests) {
        m_queueLocker.unlock();
        return false;
    }

    m_workQueue.push_back(request);

    m_queueLocker.unlock();
    m_queseStat.post();

    return true;
}

template<typename T>
void* Thread_pool<T>::worker(void* arg) {
    //静态
    Thread_pool* pool = (Thread_pool*)arg;
    pool->run();
    return pool;
}

template<class T>
void Thread_pool<T>::run() {
    while (!m_stop) {

        //从队列中取任务,等待任务
        m_queseStat.wait();
        m_queueLocker.lock();               //lock

        if (m_workQueue.empty()) {
            m_queueLocker.unlock();
            continue;
        }

        T* request = m_workQueue.front();
        m_workQueue.pop_front();

        m_queueLocker.unlock();             //unlock

        if (!request) {
            //没有获取到就继续
            continue;
        }

        request.process();

    }
}

#endif