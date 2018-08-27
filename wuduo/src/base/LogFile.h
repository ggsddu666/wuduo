#ifndef WUDUO_LOGFILE_H
#define WUDUO_LOGFILE_H

#include "Mutex.h"
#include "Types.h"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace wuduo {
namespace FileUtil {
    class AppendFile;
}

class LogFile : boost::noncopyable {
public:
    LogFile(const string &basename, off_t rollSize, bool threadSafe = true, int flushInterval = 3, int checkEveryN = 1024);
    ~LogFile();

    void append(const char *logline, int len);
    void flush();
    bool rollFile();

private:
    void append_unlocked(const char *logline, int line);

    static string getLogFileName(const string &basename, time_t * now);
    const string basename_;
    const off_t rollSize_;
    const int flushInterval_;
    const int checkEveyN_;

    int count_;
    // boost::scoped_ptr是一个比较简单的智能指针，它能保证在离开作用域之后它所管理对象能被自动释放
    // scoped_ptr不能通过其他scoped_ptr共享控制权，
    // 因为在scoped_ptr类的内部将拷贝构造函数和=运算符重载定义为私有
    boost::scoped_ptr<FileUtil::AppendFile> file_;

    const static int kRollPerSeconds_ = 60*60*24;
};
}

#endif
