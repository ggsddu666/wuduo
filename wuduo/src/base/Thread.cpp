#include "Thread.h"
#include "CurrentThread.h"
#include "Exception.h"
#include "Logging.h"

#include <sys/syscall.h>
#include <sys/prctl.h>
#include <errno.h>
#include <stdio.h>

namespace wuduo {
// 命名空间CurrentThread声明在CurrentThread.h中
namespace CurrentThread {
    // https://blog.csdn.net/liuxuejiang158blog/article/details/14100897
    // __thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。
    // __thread变量每一个线程有一份独立实体，各个线程的值互不干扰。
    // 可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。
    // (变量具体含义见CurrentThread.h)
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "unknown";
    //boost::is_same用来检测参数是否是相同的类型，相同返回true。
    const bool sameType = boost::is_same<int, pid_t>::value;
    BOOST_STATIC_ASSERT(sameType);
}

namespace detail {
pid_t gettid() {
    // tid: 线程id
    // 关于syscall(SYS_gettid)：
    // 在linux下每一个进程都一个进程id，类型pid_t，可以由getpid（）获取。
    // POSIX线程也有线程id，类型pthread_t，可以由pthread_self（）获取，线程id由线程库维护。
    // 但是各个进程独立，所以会有不同进程中线程号相同的情况。
    // 那么这样就会存在一个问题，我的进程p1中的线程pt1要与进程p2中的线程pt2通信怎么办，进程id不可以，线程id又可能重复
    // 所以这里会有一个真实的线程id唯一标识，tid。
    // glibc没有实现gettid的函数，所以我们可以通过linux下的系统调用syscall(SYS_gettid)来获得。

    // 关于syscall函数：
    //    syscall() 执行一个系统调用，根据指定的参数number和所有系统调用的汇编语言接口来确定调用哪个系统调用。
    //    系统调用所使用的符号常量可以在头文件<sys/syscll.h>里面找到
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

// fork之后打扫战场，子进程中执行
void afterFork() {
    // 为什么要赋值为0和main，因为fork可能在主线程中调用，也可能在子线程中调用。
    // fork得到一个新进程，新进程只有一个执行序列，只有一个线程

    // 1.先清零tid(线程id)
    wuduo::CurrentThread::t_cachedTid = 0;
    wuduo::CurrentThread::t_threadName = "main";

    // 2.此处再缓存tid
    CurrentThread::tid();


}

// 线程名初始化
class ThreadNameInitializer {
public:
    ThreadNameInitializer() {
        // 由下面的init全局对象先触发构造，主线程的名称为main
        wuduo::CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        // 如果我们调用了fork函数，调用成功后子进程会调用afterfork()

        // int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
        // pthread_atfork()在fork()之前调用，当调用fork时，
        // 内部创建子进程前在父进程中会调用prepare，内部创建子进程成功后，
        // 父进程会调用parent ，子进程会调用child。
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

// 全部变量类，这个对象构造先于main函数，当我们的程序引入这个库时，这个全局函数直接构造，我们程序的main()函数还没有执行。
ThreadNameInitializer init;

// 线程数据类，观察者模式
// 线程A将它传送到新开启的线程B，然后A调用函数startThread开启B线程任务，B更新线程信息后执行A传递的func
 struct ThreadData {
    typedef wuduo::Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t *tid_;
    CountDownLatch *latch_;

    ThreadData(const ThreadFunc &func, const std::string &name, pid_t *tid, CountDownLatch *latch):
               func_(func), name_(name), tid_(tid), latch_(latch) {

    }

    void runInThread() {
        *tid_ = wuduo::CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        wuduo::CurrentThread::t_threadName = name_.empty() ? "wuduoThread": name_.c_str();
        // int prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
        // prctl(PR_SET_NAME, “process_name”)
        // 第一个参数是操作类型，指定PR_SET_NAME，即设置进程名
        // 第二个参数是进程名字符串，长度至多16字节
        // https://blog.csdn.net/bad_good_man/article/details/48787031
        ::prctl(PR_SET_NAME, wuduo::CurrentThread::t_threadName);

        try {
            // 运行线程运行函数
            func_();
            wuduo::CurrentThread::t_threadName = "finished";
        } catch (const Exception &ex) {
            wuduo::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        } catch (const std::exception &ex) {
            wuduo::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        } catch (...) {
            wuduo::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
            // 再次抛出
            throw;
        }
    }
};

// 父线程交给系统的回调，obj是父线程传递的threadData
void *startThread(void *obj) {
    ThreadData *data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

}

}


using namespace wuduo;

void CurrentThread::cacheTid() {
    // 为0说明是新线程
    if(t_cachedTid == 0) {
        t_cachedTid = detail::gettid();
        // snprintf()，函数原型为int snprintf(char *str, size_t size, const char *format, ...)。
        //        将可变个参数(...)按照format格式化成字符串，然后将其复制到str中
        //        (1) 如果格式化后的字符串长度 < size，则将此字符串全部复制到str中，并给其后添加一个字符串结束符('\0')；
        //        (2) 如果格式化后的字符串长度 >= size，则只将其中的(size-1)个字符复制到str中，并给其后添加一个字符串结束符('\0')，返回值为欲写入的字符串长度。
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread() {
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec) {
    struct timespec ts = {0, 0};
    ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc &func, const std::string &n): started_(false), joined_(false), pthreadId_(0), tid_(0), func_(func), name_(n), latch_(1) {
    setDefaultName();
}

// pthread_join()和pthread_detach()都是防止现成资源泄露的途径，join()会阻塞等待。
// 这个析构函数是线程安全的。析构时确认thread没有join，才会执行析构。即线程的析构不会等待线程结束
// 如果thread对象的生命周期长于线程，那么可以通过join等待线程结束。否则thread对象析构时会自动detach线程，防止资源泄露
// 如果没有join，就detach，如果用过了，就不用了。
Thread::~Thread() {
    if(started_ && !joined_) {
        // pthread_detach用于分离
        // 一个分离的线程是不能被其他线程回收或杀死的，它的存储器资源在它终止时由系统自动释放。
        pthread_detach(pthreadId_);
    }
}

void Thread::setDefaultName() {
    int num = numCreated_.incrementAndGet();
    if(name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

void Thread::start() {
    assert(!started_);
    started_ = true;

    detail::ThreadData *data = new detail::ThreadData(func_, name_, &tid_, &latch_);
    if(pthread_create(&pthreadId_, NULL, &detail::startThread, data)) {
        // 开启失败
        started_ = false;
        delete data;
        // 暂时不用Logging中的函数
        fprintf(stderr, "Failed in pthread_create");
    } else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}





