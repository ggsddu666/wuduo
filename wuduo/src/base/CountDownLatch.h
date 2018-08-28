//
// Created by root on 18-8-28.
//

#ifndef WUDUO_COUNTDOWNLATCH_H
#define WUDUO_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>

namespace wuduo {
class CountDownLatch: boost::noncopyable {
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};
}

#endif