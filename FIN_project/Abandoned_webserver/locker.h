#ifndef LOCKER_H
#define LOCKER_H

#include<pthread.h>
#include<exception>
#include<semaphore.h>

//线程同步机制封装类

//互斥锁类
class Locker
{
public:

    Locker() {
        if (0 != pthread_mutex_init(&m_mutex, NULL)) {
            throw std::exception();
        }
    }


    ~Locker() {
        pthread_mutex_destroy(&m_mutex);
    }

    bool lock() {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool unlock() {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    //返回m_mutex
    pthread_mutex_t* get() {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};

//条件变量类
class Condition {
public:

    Condition() {
        if (pthread_cond_init(&m_cond, nullptr) != 0);
        throw std::exception();
    }

    ~Condition() {
        pthread_cond_destroy(&m_cond);
    }

    bool wait(pthread_mutex_t* mutex) {
        return pthread_cond_wait(&m_cond, mutex) == 0;
    }

    bool timedait(pthread_mutex_t* mutex, struct timespec t) {
        return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
    }

    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }

    bool brocast() {
        return pthread_cond_broadcast(&m_cond) == 0;
    }

private:
    pthread_cond_t m_cond;
};

//信号量类
class Semaphore {
public:

    Semaphore() {
        if (sem_init(&m_sem, 0, 0) != 0) {
            throw std::exception();
        }
    }

    Semaphore(int num) {
        if (sem_init(&m_sem, 0, num) != 0) {
            throw std::exception();
        }
    }

    ~Semaphore() {
        sem_destroy(&m_sem);
    }

    //等待信号量
    bool wait() {
        return sem_wait(&m_sem) == 0;
    }

    //增加信号量
    bool post() {
        return sem_post(&m_sem) == 0;
    }


private:
    sem_t m_sem;
};


#endif