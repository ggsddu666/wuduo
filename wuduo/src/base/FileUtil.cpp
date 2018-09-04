#include "FileUtil.h"
#include "Types.h"

#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <boost/static_assert.hpp>




using namespace wuduo;

FileUtil::AppendFile::AppendFile(StringArg filename): fp_(fopen(filename.c_str(), "ae")),
                                                      writtenBytes_(0) {
    assert(fp_);
    setbuffer(fp_, buffer_, sizeof buffer_);
}

FileUtil::AppendFile::~AppendFile() {
    fclose(fp_);
}

void FileUtil::AppendFile::append(const char *logline, const size_t len) {
    size_t n = write(logline, len);
    size_t remain = len - n;
    while(remain > 0) {
        size_t x = write(logline + n, remain);
        if(x == 0) {
            int err = ferror(fp_);
            if(err) {
                // strerror_tl在logging头文件中
                // fprintf(stderr, "AppendFile::append() faild %s\n", strerror_tl(err));
                fprintf(stderr, "AppendFile::append() faild %d\n", err);
            }

            break;
        }

        n += x;
        remain = len - n;
    }

    writtenBytes_ += len;
}

void FileUtil::AppendFile::flush() {
    ::fflush(fp_);
}

size_t FileUtil::AppendFile::write(const char *logline, size_t len) {
    // 为了快速，使用unlocked(无锁)的fwrite函数。平时我们使用的C语言IO函数，都是线程安全的，
    // 为了做到线程安全，会在函数的内部加锁。这会拖慢速度。而对于这个类。可以保证，从
    // 始到终只有一个线程能访问，所以无需进行加锁操作
    // size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
    //    （1）buffer：是一个指针，对fwrite来说，是要获取数据的地址；
    //    （2）size：要写入内容的单字节数；
    //    （3）count:要进行写入size字节的数据项的个数；
    //    （4）stream:目标文件指针；
    // fwrite_unlocked参数同fwrite
    return ::fwrite_unlocked(logline, 1, len, fp_);
}



FileUtil::ReadSmallFile::ReadSmallFile(StringArg filename): fd_(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
                                                            err_(0) {
    buf_[0] = '\0';
    if(fd_ < 0) {
        err_ = errno;
    }
}

FileUtil::ReadSmallFile::~ReadSmallFile() {
    if(fd_ >= 0) {
        ::close(fd_);
    }
}

// 读文件，长度保存在fileSize，内容保存在content, 最大长度不超过fileSize
template <typename String>
int FileUtil::ReadSmallFile::readToString(int maxSize,
                                          String *content,
                                          int64_t *fileSize,
                                          int64_t *modifyTime,
                                          int64_t *createTime) {
    BOOST_STATIC_ASSERT(sizeof(off_t) == 8);
    assert(content != NULL);
    int err = err_;
    if(fd_ >= 0) {
        content->clear();

        if(fileSize) {
            struct stat statbuf;
            // fstat取得文件状态
            if(::fstat(fd_, &statbuf) == 0) {
                // S_ISREG是否是一个常规文件.
                if(S_ISREG(statbuf.st_mode)) {
                    *fileSize = statbuf.st_size;
                    content->reserve(static_cast<int>(std::min(implicit_cast<int64_t>(maxSize), *fileSize)));
                } else if(S_ISDIR(statbuf.st_mode)) {
                    err = EISDIR;
                }

                if(modifyTime) {
                    *modifyTime = statbuf.st_mtime;
                }

                if(createTime) {
                    *createTime = statbuf.st_ctime;
                }
            } else {
                err = errno;
            }
        }

        // 也就是上面的比较结果，可读的最大值超过文件大小，那就尝试读到最大
        // 即如果maxSize > fileSize, 则不断读文件内容，可以读完整个文件（因为此时content->reserve的大小就是文件的大小）
        while(content->size() < implicit_cast<size_t >(maxSize)) {
            // 思想就是尽量多地读
            size_t toRead = std::min(implicit_cast<size_t >(maxSize) - content->size(), sizeof(buf_));
            ssize_t n = ::read(fd_, buf_, toRead);

            // 读出n字节后，追加到content
            if(n > 0) {
                content->append(buf_, n);
            } else {
                if(n < 0) {
                    err = errno;
                }

                // 即 n = 0 数据读完的情况
                break;
            }
        }
    }

    return err;
}

int FileUtil::ReadSmallFile::readToBuffer(int *size) {
    int err = err_;
    if(fd_ >= 0) {
        // ssize_t pread(intfd, void *buf, size_tcount, off_toffset);
        // pread函数相当于先后调用了lseek和read函数，但是还是有区别的，有以下两点区别：
        // pread函数是原子操作，而先后调用两个函数不是原子操作
        // pread函数是不会改变当前文件偏移量的，而read和write函数会改变当前文件偏移量
        ssize_t n = ::pread(fd_, buf_, sizeof(buf_)-1, 0);
        if(n >= 0) {
            if(size) {
                *size = static_cast<int>(n);
            }
            buf_[n] = '\0';
        } else {
            err = errno;
        }
    }

    return err;

}

//      int readToString(int maxSize,
//                     String *content,
//                     int64_t *fileSize,
//                     int64_t *modifyTime,
//                     int64_t *createTime);