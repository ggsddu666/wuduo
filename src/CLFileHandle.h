#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include "unistd.h"
#include <string.h>

class CLFileHandle {
    public:
        CLFileHandle(const char* FileName);
        ~CLFileHandle();
        // 读文件：从pos位置开始读入len长度
        int ReadFromFile(int pos, int len);
        // 写文件：将pstrMsg字符串写入到文件中
        int WriteToFile(const char* pstrMsg);
    private:
        CLFileHandle();
        // 刷新缓存
        int flushCache();

        // 文件名
        const char* FileName;
        // 文件标识符
        int fd;
        // 互斥信号量
        pthread_mutex_t* mutex;
        // 缓存区
        char* buffer;
        // 缓存区已使用长度
        unsigned int UsedOfBuffer;


};