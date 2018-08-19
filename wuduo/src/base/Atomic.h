//
// Created by root on 18-8-15.
//

#ifndef WUDUO_ATOMIC_H
#define WUDUO_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

// 功能：实现对数据类型的原子操作

namespace wuduo {
namespace detail {
template<typename T>
class AtomicIntererT: boost::noncopyable {
    // noncopyable : 单例，除非自己定义，否则外部不能进行拷构造等操作
public:
    AtomicIntererT(): value_(0) {}

    // 返回value_的值，如果value_=0，把它和0交换。
    T get() {
        // type __sync_val_compare_and_swap (type *ptr, type oldval, type newval, ...)
        // 提供原子的比较和交换，如果*ptr == oldval,就将newval写入*ptr
        // 返回操作之前的值。
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    // 返回value更新前的值，即返回value
    T getAndAdd(T X) {
        return __sync_fetch_and_add(&value_, X);
    }

    T addAndGet(T X) {
        return getAndAdd(X) + X;
    }

    T incrementAndGet() {
        return addAndGet(1);
    }

    T decrementAndGet() {
        return addAndGet(-1);
    }

    void add(T X) {
        addAndGet(X);
    }

    void increment() {
        addAndGet(1);
    }

    void decrement() {
        addAndGet(-1);
    }

    // 将value设置为newValie，返回操作前value的值
    T getAndSet(T newValue) {
        return __sync_lock_test_and_set(&value_, newValue);
    }

private:
    volatile T value_;
};
}

typedef detail::AtomicIntererT<int32_t > AtomicInt32;
typedef detail::AtomicIntererT<int64_t > AtomicInt64;



}

#endif //WUDUO_ATOMIC_H
