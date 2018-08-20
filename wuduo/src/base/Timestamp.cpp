//
// Created by wu on 2018/8/19.
//

#include "Timestamp.h"
#include <cstdio>
#include <boost/static_assert.hpp>
#include <sys/time.h>


using namespace wuduo;

// 若使用PRID64，需打开这个宏
// 需在include <inttypes.h>之前
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

// 编译时断言通过
BOOST_STATIC_ASSERT(sizeof(Timestamp) == sizeof(int64_t));

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

string Timestamp::toString() const {
    // （buf可以再小点）
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    // PRId64跨平台打印64位整数，因为int64_t用来表示64位整数，在32位系统中是long long int，64位系统中是long int
    // 所以打印64位是%ld或%lld，可移植性较差，不如统一同PRID64来打印。
    snprintf(buf, sizeof(buf) - 1 , "%" PRId64 ".%010" PRId64 "", seconds, microseconds);

    return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds) const {
    // char buf[25] = {0};
    char buf[64] = {0};
    time_t seconds = static_cast<time_t > (microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    struct tm tm_time;

    // struct tm *gmtime(const time_t *timep);
    // struct tm *gmtime_r(const time_t *timep, struct tm *result);
    // gmtime(线程不安全的)是把日期和时间转换为格林威治(GMT)时间的函数。
    // 将参数timep 所指的time_t 结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果由结构tm返回。
    // 使用gmtime后要立即处理结果，否则返回的指针指向的内容可能会被覆盖。一个好的方法是使用gmtime_r（线程安全的）
    // gmtime_r()函数功能与此相同，但是它可以将数据存储到用户提供的结构体中，由于使用了用户分配的内存，是不会出错的。

    // 把time_t结构中的信息转换成真实世界所使用的时间日期，存储在tm_time结构中
    gmtime_r(&seconds, &tm_time);
    // 是否打印出微秒
    if(showMicroseconds) {
        int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds
        );
    } else {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec
        );
    }
    return buf;
}



