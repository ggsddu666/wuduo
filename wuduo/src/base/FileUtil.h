//
// Created by wu on 18-8-28.
//

#ifndef WUDUO_FILEUTIL_H
#define WUDUO_FILEUTIL_H

#include "StringPiece.h"
#include <boost/noncopyable.hpp>
#include <sys/types.h>

namespace wuduo {
namespace FileUtil {
// 读的文件 < 64kB
class ReadSmallFile: boost::noncopyable {
public:
    ReadSmallFile(StringArg filename);
    ~ReadSmallFile();

    template <typename String>
    int readToString(int maxSize,
                     String *content,
                     int64_t *fileSize,
                     int64_t *modifyTime,
                     int64_t *createTime);

    int readToBuffer(int *size);
    const char* buffer() const { return buf_; }

    static const int kBufferSize = 64 * 1024;

private:
    int fd_;
    int err_;
    char buf_[kBufferSize];
};

template <typename String>
int readFile(StringArg filename,
             int maxSize,
             String *content,
             int64_t *fileSize,
             int64_t *modifyTime,
             int64_t *createTime) {
    ReadSmallFile file(filename);
    return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

class AppendFile: boost::noncopyable {
public:
    explicit AppendFile(StringArg filename);
    ~AppendFile();

    void append(const char *logline, const size_t len);
    void flush();
    off_t writtenBytes() const { return writtenBytes_; }

private:
    size_t write(const char *logline, size_t len);
    FILE *fp_;
    char buffer_[60*1024];
    off_t writtenBytes_;
};

}
}


#endif