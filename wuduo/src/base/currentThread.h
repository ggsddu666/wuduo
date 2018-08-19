//
// Created by root on 18-8-15.
//

#ifndef WUDUO_CURRENTTHREAD_H
#define WUDUO_CURRENTTHREAD_H

#include <stdint.h>
/*
 * __thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。
 * __thread变量每一个线程有一份独立实体，各个线程的值互不干扰。
 * 例如t_cachedTid这个变量，每个线程都有，但是每个线程的t_cachedTid是独立的，也就是一个线程只能修改它自己线程的t_cachedTid，而不能修改其他线程的t_cachedTid
 * 可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。
 */


namespace wuduo {
namespace currentThread {
    // internal
    // 线程id
    extern __thread int t_cachedTid;
    // id的字符串形式
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    // 线程的名字
    extern __thread const char* t_threadName;
    // 缓存线程的id（在不同的线程中，会缓存各自线程对应的id）
    void cacheTid();

    inline int tid() {
        // __builtin_expect作用是"允许程序员将最有可能执行的分支告诉编译器"。这个指令的写法为：__builtin_expect(EXP, N)。
        // 意思是：EXP==N的概率很大。一般的使用方法是将__builtin_expect指令封装为LIKELY和UNLIKELY宏。

        /*
         * __builtin_expect实际上是为了满足在大多数情况不执行跳转指令，
         * 所以__builtin_expect仅仅是告诉编译器优化，并没有改变其对真值的判断
         */
        if(__builtin_expect(t_cachedTid == 0, 0)) {
            // 缓存线程id
            cacheTid();
        }

        return t_cachedTid;
    }

    inline const char* tidString() {
        return t_tidString;
    }

    inline int tidStringLength() {
        return t_tidStringLength;
    }

    inline const char* name() {
        return t_threadName;
    }

    bool isMainThread();

    void sleepUsec(int64_t usec);
}
}


#endif //WUDUO_CURRENTTHREAD_H
