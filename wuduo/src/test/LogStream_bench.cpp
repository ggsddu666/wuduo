#include "base/LogStream.h"
#include "base/Timestamp.h"


// #include "base/Types.h"
#include <stdio.h>
#include <sstream>
#include <inttypes.h>

using namespace wuduo;
const size_t N = 1000000;


template <typename T>
void benchPrintf(const char *fmt) {
    char buf[32];
    Timestamp start(Timestamp::now());
    for(size_t i = 0; i < N; ++i) {
        snprintf(buf, sizeof(buf), fmt, (T)(i));
    }
    Timestamp end(Timestamp::now());

    printf("benchPrintf %f\n", timeDifference(end, start));
}

template <typename T>
void benchStringStream() {
    Timestamp start(Timestamp::now());
    std::ostringstream os;

    for(int i = 0; i < N; ++i) {
        os << (T)(i);
        // seekp:第一个参数是偏移量, 第二个参数是基地址。
        // beg: 以查找 （改变当前的读取或写入位置） 相对于 （数组、 流或文件） 的序列开头。
        // 这句话：想把输入流定位到流的开始位置
        os.seekp(0, std::ios_base::beg);
    }
    Timestamp end(Timestamp::now());

    printf("benchStringStream %f\n", timeDifference(end, start));
}

// 与上一个原生进行比较
template <typename T>
void benchLogStream() {
    Timestamp start(Timestamp::now());
    LogStream os;
    for(int i = 0; i < N; ++i) {
        os << (T)(i);
        os.resetBuffer();
    }
    Timestamp end(Timestamp::now());

    printf("benchLogStream %f\n", timeDifference(end, start));
}


int main() {
    benchPrintf<int>("%d");

    puts("int");
    benchPrintf<int>("%d");
    benchStringStream<int>();
    benchLogStream<int>();

    puts("double");
    benchPrintf<double>("%.12g");
    benchStringStream<double>();
    benchLogStream<double>();

    puts("int64_t");
    benchPrintf<int64_t>("%" PRId64);
    benchStringStream<int64_t>();
    benchLogStream<int64_t>();

    puts("void*");
    benchPrintf<void*>("%p");
    benchStringStream<void*>();
    benchLogStream<void*>();
    return 0;
}