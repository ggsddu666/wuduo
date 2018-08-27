//
// Created by root on 18-8-15.
//

#ifndef WUDUO_MUTEX_H
#define WUDUO_MUTEX_H

#include "currentThread.h"
#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE

//#ifdef NDEBUG
// 跳过
#endif


// 检查是否出错 ，__typeof__定义与ret类型相同的errnum，大多数情况下不执行if语句（大多数情况下errnum != 0 为假，即errnum = 0）
#define MCHECK(ret) ({ __typeof__(ret) errnum = (ret);          \
                       if(__builtin_expect(errnum != 0, 0))     \
                            __assert_perror_fail(errnum, __FILE__, __LINE__, __func__); \
})



namespace wuduo {
class MutexLock: boost::noncopyable {
public:
    MutexLock(): holder_(0) {
        // MCHECK相当于assert
        MCHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MutexLock() {
        // 只有在没有被其它线程持有的情况下才可以析构
        assert(holder_ == 0);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    // 是否被本线程上锁(互斥锁是否作用于当前线程)
    bool isLockedByThisThread() const {
        return holder_ == currentThread::tid();
    }

    // 如果不是作用于当前线程，则退出
    void assertLocked() const {
        assert(isLockedByThisThread());
    }

    void lock() {
        MCHECK(pthread_mutex_lock(&mutex_));
        assignHolder();
    }

    void unlock() {
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&mutex_));
    }

    pthread_mutex_t* getPthreadMutex() {
        return &mutex_;
    }



private:
    friend class Condition;

// 内部类
class UnassignGuard: boost::noncopyable {
public:
    UnassignGuard(MutexLock& owner): owner_(owner) {
        owner_.unassignHolder();
    }

    ~UnassignGuard() {
        owner_.assignHolder();
    }

private:
    MutexLock& owner_;
};

    void unassignHolder() {
        holder_ = 0;
    }

    void assignHolder() {
        holder_ = currentThread::tid();
    }

    pid_t holder_;
    pthread_mutex_t mutex_;
};

// MutexLock 和 MutexLockGuard 不存在整体与局部关系（聚合），只存在关联关系。MutexLockGuard 生存期结束的时候，MutexLock 的生存期还没结束
// 如果两个类之间存在整体与局部关系，其中一个类还负责另一个类的构造与销毁（生存期），则为组合关系。

class MutexLockGuard: boost::noncopyable {
public:
    explicit MutexLockGuard(MutexLock& mutex): mutex_(mutex) {
        mutex_.lock();
    }

    ~MutexLockGuard() {
        mutex_.unlock();
    }

private:
    // 为甚么要使用引用？因为他们仅仅是关联关系，使用引用不会导致MutexLock对象的销毁！！！
    MutexLock& mutex_;
};
}

// 定义一个宏，防止定义一个无名临时MutexLockGuard对象，因为它不能够长时间拥有锁
#define MutexLockGuard(x) error "Missing guard object name"


#endif //WUDUO_MUTEX_H
