#include "base/CurrentThread.h"
#include "base/Thread.h"
#include "base/Mutex.h"
#include "base/Timestamp.h"

#include <iostream>
#include <map>
#include <stdio.h>
#include <boost/bind.hpp>
#include <sys/types.h>
#include <sys/wait.h>

wuduo::MutexLock g_mutex;
std::map<int, int> g_delays;

void threadFunc() {

}

void threadFunc2(wuduo::Timestamp start) {
    wuduo::Timestamp now(wuduo::Timestamp::now());
    int delay = static_cast<int>(timeDifference(now, start) * 1000000);
    wuduo::MutexLockGuard lock(g_mutex);
    ++g_delays[delay];
}

// 测试fork的性能
void forkBench() {
    sleep(10);
    wuduo::Timestamp start(wuduo::Timestamp::now());
    int kProcesses = 10*1000;

    for(int i = 0; i < kProcesses; ++i) {
        //  在父进程中，fork返回新创建子进程的进程ID；
        //  在子进程中，fork返回0；
        pid_t child = fork();
        if(child == 0) {
            exit(0);
        } else {
            waitpid(child, NULL, 0);
        }
    }

    double timeUsed = timeDifference(wuduo::Timestamp::now(), start);
   // printf("1.process creation time used %f\n", timeUsed*1000000);
    printf("2.process creation time used %f\n", timeUsed*1000000/kProcesses);
    printf("number of created processes %d\n", kProcesses);
}


int main() {
    printf("pid = %d, tid = %d\n", ::getpid(), wuduo::CurrentThread::tid());
    wuduo::Timestamp start(wuduo::Timestamp::now());

    int kThreads = 100*1000;
    for(int i = 0; i < kThreads; ++i) {
        wuduo::Thread t1(threadFunc);
        t1.start();
        t1.join();
    }

    double timeUsed = timeDifference(wuduo::Timestamp::now(), start);
    printf("1.thread creation time %f \n", timeUsed);
    printf("2.thread creation time %f \n", timeUsed/kThreads);
    printf("3.thread creation time %f \n", timeUsed*1000000/kThreads);
    printf("number of created threads %d\n", wuduo::Thread::numCreated());

    for(int i = 0; i < kThreads; ++i) {
        wuduo::Timestamp now(wuduo::Timestamp::now());
        wuduo::Thread t2(boost::bind(threadFunc2, now));
        t2.start();
        t2.join();
    }

    {
        wuduo::MutexLockGuard lock(g_mutex);
        for(std::map<int, int>::iterator it = g_delays.begin(); it != g_delays.end(); ++it) {
            printf("delay = %d, count = %d\n", it->first, it->second);
        }
    }

    forkBench();
    return 0;
}