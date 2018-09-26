//
// Created by root on 18-8-16.
//

#ifndef WUDUO_LOGGING_H
#define WUDUO_LOGGING_H

// Logger类用枚举类型定义了日志等级。

#include <cstring>
#include "LogStream.h"
#include "Timestamp.h"

namespace wuduo {
class TimeZone;

class Logger {
public:
    // log等级
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    class SourceFile {
    public:
        // template <int N> : 实现定义不同长度的数组
        template <int N>
        inline SourceFile(const char (&arr)[N]): data_(arr), size_(N - 1) {
            // char *strrchr(const char *str, char c);
            // 找一个字符c在另一个字符串str中末次出现的位置
            const char *slash = strrchr(data_, '/');
            if(slash) {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char * filename):  data_(filename){
            data_ = filename;
            const char *slash = strrchr(data_, '/');
            if(slash) {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }


        const char *data_;
        int size_;

    };


    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char *func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream& stream() {
        return impl_.stream_;
    }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char *msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
    static void setTimeZone(const TimeZone &tz);


private:
class Impl {
public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level, int old_errno, const SourceFile &file, int line);
    void formatTime();
    void finish();

    Timestamp time_;
    LogStream stream_;
    LogLevel level_;
    int line_;
    SourceFile basename_;

};

    Impl impl_;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel() {
    return g_logLevel;
}

#define LOG_TRACE if (wuduo::Logger::logLevel() <= wuduo::Logger::TRACE) \
  wuduo::Logger(__FILE__, __LINE__, wuduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (wuduo::Logger::logLevel() <= wuduo::Logger::DEBUG) \
  wuduo::Logger(__FILE__, __LINE__, wuduo::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (wuduo::Logger::logLevel() <= wuduo::Logger::INFO) \
  wuduo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN wuduo::Logger(__FILE__, __LINE__, wuduo::Logger::WARN).stream()
#define LOG_ERROR wuduo::Logger(__FILE__, __LINE__, wuduo::Logger::ERROR).stream()
#define LOG_FATAL wuduo::Logger(__FILE__, __LINE__, wuduo::Logger::FATAL).stream()
#define LOG_SYSERR wuduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL wuduo::Logger(__FILE__, __LINE__, true).stream()

const char *strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
  ::wuduo::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T *ptr) {
    if(ptr == NULL) {
        Logger(file, line, Logger::FATAL).stream() << names;
    }
}
}


#endif //WUDUO_LOGGING_H
