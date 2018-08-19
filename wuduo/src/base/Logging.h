//
// Created by root on 18-8-16.
//

#ifndef WUDUO_LOGGING_H
#define WUDUO_LOGGING_H

// Logger类用枚举类型定义了日志等级。

#include <cstring>

namespace wuduo {
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


private:
class Impl {
public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level, int old_errno, const SourceFile &file, int line);
    void formatTime();
    void finish();

    // Timestamp time_;


};

};

}


#endif //WUDUO_LOGGING_H
