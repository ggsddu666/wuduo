//
// Created by wu on 2018/8/22.
//

#ifndef WUDUO_LOGSTREAM_CPP
#define WUDUO_LOGSTREAM_CPP

#include "LogStream.h"
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
//#include<stdexcept>
#include<limits>

#include <algorithm>

//#include <assert.h>
//#include <string.h>
//#include <stdint.h>
#include <stdio.h>

//
//#if defined(__clang__)
//#pragma clang diagnostic ignored "-Wtautological-compare"
//#else
//#pragma GCC diagnostic ignored "-Wtype-limits"
//#endif

using namespace wuduo;
using namespace wuduo::detail;

namespace wuduo {
namespace detail {
    // zero两边对称，因为余数可能为负数
    const char digits[] = "9876543210123456789";
    const char *zero = digits + 9;
    BOOST_STATIC_ASSERT(sizeof(digits) == 20);

    const char digitsHex[] = "0123456789ABCDEF";
    BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

    // 数字(整数)转字符串
    template <typename T>
    size_t convert(char buf[], T value) {
        T i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while(i != 0);

        if(value < 0) {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    // 关于uintptr_t指针
    // intptr_t 和uintptr_t 类型用来存放指针地址
    // 原因：指针在32位是4字节，在64位是8字节。。。
    // 而intptr_t在32位是int，uintptr_t在64位是long int，刚好分别是4字节和8字节
    // long同指针一样，也是32位是4字节，在64位是8字节。。
    // https://blog.csdn.net/cs_zhanyb/article/details/16973379
    size_t convertHex(char buf[], uintptr_t value) {
        uintptr_t i = value;
        char *p = buf;

        do {
            int lsd = static_cast<int>(i % 16);
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);

        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    template class FixedBuffer<kSmallBuffer>;
    template class FixedBuffer<kLargeBuffer>;
}
}

// 在数据后面加个\0，相当于字符串
template <int SIZE>
const char *FixedBuffer<SIZE>::debugString() {
    *cur_ = '\0';
    return data_;
}

template <int SIZE>
void FixedBuffer<SIZE>::cookieStart() {

}

template <int SIZE>
void FixedBuffer<SIZE>::cookieEnd() {

}

// 静态检查，用于检查一些类型的大小
//  std::numeric_limits<T>::digits10 ，返回目标类型在十进制下可以表示的最大位数，
void LogStream::staticCheck() {
    BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<double >:: digits10);
    BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10);
    BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10);
    BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10);
}

//  通过模版函数formatInteger()把
//  short、unsigned short、int、unsigned int、long、unsigned long、long long等类型转换为字符串，并保存到buffer中。
template <typename T>
void LogStream::formatInteger(T v) {
    if(buffer_.avail() >= kMaxNumericSize) {
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(const void *p) {
    uintptr_t v = reinterpret_cast<uintptr_t >(p);
    if(buffer_.avail() >= kMaxNumericSize) {
        char *buf = buffer_.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = convertHex(buf+2, v);
        buffer_.add(len + 2);
    }

    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if(buffer_.avail() >= kMaxNumericSize) {
        // ％g根据大小自动选f格式或e格式，且不输出无意义的零。
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.add(len);
    }
}

template <typename T>
Fmt::Fmt(const char *fmt, T val) {
    // 如果T是（可能是cv限定的）算术类型，则继承自 true_type，
    // 否则继承自 false_type
    // 断言是算术类型（即整数类型或浮点类型）
    BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value == true);

    length_ = snprintf(buf_, sizeof buf_, fmt, val);
    assert(static_cast<size_t >(length_) < sizeof buf_);

}

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);







#endif