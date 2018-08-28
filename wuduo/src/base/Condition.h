//
// Created by root on 18-8-15.
//

#ifndef WUDUO_CONDITION_H
#define WUDUO_CONDITION_H

#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace wuduo {
class Condition: boost::noncopyable {
public:
    explicit Condition(MutexLock &mutex): mutex_(mutex) {
        MCHECK(pthread_cond_init(&pcond_, NULL));
    }

    ~Condition() {
        MCHECK(pthread_cond_destroy(&pcond_));
    }

    void wait() {
        // 在执行等待之前，使用UnassignGuard的构造函数将mutex_的holder清空
        // 因为当前线程会休眠，暂时失去对mutex_的所有权。
        // 接着调用pthread_cond_wait等待其他线程的通知。
        // 当其他某个线程调用了notify/notifyAll时，当前线程被唤醒，接着在wait返回时
        // UnassignGuard的析构函数自动将mutex_的holder设置为当前线程。
        MutexLock::UnassignGuard ug(mutex_);
        MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
    }

    bool waitForSeconds(double seconds);

    void notify() {
        MCHECK(pthread_cond_signal(&pcond_));
    }

    void notifyAll() {
        MCHECK(pthread_cond_broadcast(&pcond_));
    }

private:
    MutexLock &mutex_;
    pthread_cond_t pcond_;
};
}


#endif //WUDUO_CONDITION_H
