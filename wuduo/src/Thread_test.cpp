#include "base/Thread.h"
#include <stdio.h>
#include <boost/bind.hpp>

void mysleep(int seconds) {
    // int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
    //    nanosleep()函数会导致当前的线程将暂停执行,直到rqtp参数所指定的时间间隔。或者在指定时间间隔内有信号传递到当前线程，将引起当前线程调用信号捕获函数或终止该线程。
    //    　　暂停时间可能超过请求时间,因为参数值是sleep粒度的整数倍数或者因为其他活动的系统调度。
    //    　　但是,除了被信号中断的情况下,暂停时间不会少于rqtp指定的时间,由系统时钟CLOCK_REALTIME测量。
    //    　　使用nanosleep()函数对其他行为没有影响，不堵塞任何信号。
    timespec t = { seconds, 0 };
    nanosleep(&t, NULL);
}

void threadFunc() {
    printf("tid = %d\n", wuduo::CurrentThread::tid());
}

void threadFunc2(int x) {
    printf("tid=%d, x=%d\n", wuduo::CurrentThread::tid(), x);
}

void threadFunc3() {
    printf("tid=%d\n", wuduo::CurrentThread::tid());
    mysleep(1);
}

class Foo {
public:
    explicit Foo(double x) : x_(x) { }

    void memberFunc() {
        printf("tid=%d, Foo::x_=%f\n", wuduo::CurrentThread::tid(), x_);
    }

    void memberFunc2(const std::string& text) {
        printf("tid=%d, Foo::x_=%f, text=%s\n", wuduo::CurrentThread::tid(), x_, text.c_str());
    }

private:
    double x_;
};

int main() {
    printf("pid = %d, tid = %d\n", ::getpid(), wuduo::CurrentThread::tid());

    wuduo::Thread t1(threadFunc);
    t1.start();
    printf("t1.tid = %d\n", t1.tid());
    t1.join();

    wuduo::Thread t2(boost::bind(threadFunc2, 42), "thread for free function with argument");
    t2.start();
    printf("t2.tid = %d\n", t2.tid());
    t2.join();

    Foo foo(87.53);
    wuduo::Thread t3(boost::bind(&Foo::memberFunc, &foo), "thread for member function without argument");
    t3.start();
    t3.join();

    // boost::ref
    // https://blog.csdn.net/fengbangyue/article/details/7173697
    wuduo::Thread t4(boost::bind(&Foo::memberFunc2, boost::ref(foo), std::string("test string")));
    t4.start();
    t4.join();

    {
        wuduo::Thread t5(threadFunc3);
        t5.start();
        // t5 may destruct eariler than thread creation.
    }
    mysleep(2);
    {
        wuduo::Thread t6(threadFunc3);
        t6.start();
        mysleep(2);
        // t6 destruct later than thread creation.
    }
    sleep(2);
    printf("number of created threads %d\n", wuduo::Thread::numCreated());
    return 0;
}