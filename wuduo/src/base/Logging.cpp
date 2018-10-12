//
// Created by root on 18-8-16.
//

#include "Logging.h"
#include "Timestamp.h"
#include "TimeZone.h"
#include "CurrentThread.h"

#include <errno.h>
#include <stdio.h>

namespace wuduo {

// 关于gcc thread：https://blog.csdn.net/huangjh2017/article/details/74829265
// __thread是GCC内置的线程局部存储设施，其存储效率可以和全局变量相比；
// __thread变量在每一个线程中都有一份独立实例，各线程值是互不干扰的。

// 同时__thread关键字的使用也有自身的限制：
// 只能修饰POD类型
// （类似整型指针的标量，不带自定义的构造、拷贝、赋值、析构的类型，二进制内容可以任意复制memset,memcpy,且内容可以复原）。
// 不能修饰class类型，因为无法自动调用构造和析构函数。
// 可用于修饰全局变量，函数内的静态变量，不能修饰函数的局部变量或class的普通成员变量。
// 且__thread变量值只能初始化为编译器常量。

__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno) {
    return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
}

Logger::LogLevel initLogLevel() {
    // char *getenv(const char *name) 搜索 name 所指向的环境字符串，并返回相关的值给字符串。
    if(::getenv("WUDUO_LOG_TRACE"))
        return Logger::TRACE;
    else if(::getenv("WUDUO_LOG_DEBUG"))
        return Logger::DEBUG;
    else
        return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
        "TRACE ",
        "DEBUG ",
        "INFO  ",
        "WARN  ",
        "ERRNO ",
        "FATAL "
};

class T {
public:
    T(const char *str, unsigned len): str_(str), len_(len) {
        assert(strlen(str) == len_);
    }

    const char *str_;
    const unsigned len_;
};

inline LogStream& operator<<(LogStream &s, T v) {
    s.append(v.str_, v.len_);
    return s;
}

inline LogStream& operator<<(LogStream &s, const Logger::SourceFile &v) {
    s.append(v.data_, v.size_);
    return s;
}

void defaultOutput(const char *msg, int len) {
    size_t n = fwrite(msg, 1, len, stdout);
    (void)n;
}

void defaultFlush() {
    fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
TimeZone g_logTimeZone;

}

using namespace wuduo;

Logger::Impl::Impl(wuduo::Logger::LogLevel level, int savedErrno, const wuduo::Logger::SourceFile &file, int line)
                 : time_(Timestamp::now()),
                   stream_(),
                   level_(level),
                   line_(line),
                   basename_(file) {
    formatTime();
    CurrentThread::tid();
    stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
    stream_ << T(LogLevelName[level], 6);
    if(savedErrno != 0) {
        stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << "）";
    }
}

void Logger::Impl::formatTime() {
    // 将time_格式化
    int64_t microSencondsSinceEpoch = time_.microSecondsSinceEpoch();
    time_t seconds = static_cast<time_t>(microSencondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
    int microseconds = static_cast<int>(microSencondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);

    // 更新t_lastSecond
    if(seconds != t_lastSecond) {
        t_lastSecond = seconds;
        struct tm tm_time;
        if(g_logTimeZone.valid()) {
            tm_time = g_logTimeZone.toLocalTime(seconds);
        } else {
            ::gmtime_r(&seconds, &tm_time);
        }

        int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d", tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

        assert(len == 17);

        // ???
        (void)len;
    }

    if(g_logTimeZone.valid()) {
        // 定义在logstream中
        Fmt us(".06%d ", microseconds);
        assert(us.length() == 8);
        stream_ << T(t_time, 17) << T(us.data(), 8);
    } else {
        Fmt us(".06%dZ ", microseconds);
//        std::cout << us.data() << std::endl;
//        std::cout << us.length() << std::endl;
//
//        assert(us.length() == 9);
      //  stream_ << T(t_time, 17) << T(us.data(), 9);
        stream_ << T(t_time, 17) << T(us.data(), us.length());
    }

}

void Logger::Impl::finish() {
    stream_ << " - " << basename_ << ';' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line): impl_(INFO, 0, file, line) {

}

Logger::Logger(SourceFile file, int line, LogLevel level, const char *func): impl_(level, 0, file, line) {
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level): impl_(level, 0, file, line) {

}

Logger::Logger(SourceFile file, int line, bool toAbort): impl_(toAbort ? FATAL : ERROR, errno, file, line) {

}

Logger::~Logger() {
    impl_.finish();
    const LogStream::Buffer& buf(stream().buffer());
    g_output(buf.data(), buf.length());
    if(impl_.level_ == FATAL) {
        g_flush();
        abort();
    }
}

void Logger::setLogLevel(Logger::LogLevel level) {
    g_logLevel = level;
}

void Logger::setOutput(OutputFunc out) {
    g_output = out;
}

void Logger::setFlush(FlushFunc flush) {
    g_flush = flush;
}

void Logger::setTimeZone(const TimeZone& tz) {
    g_logTimeZone = tz;
}

