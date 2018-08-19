//
// Created by wu on 2018/8/19.
//

#ifndef WUDUO_TIMESTAMP_H
#define WUDUO_TIMESTAMP_H

#include "copyable.h"
#include "Types.h"
#include <boost/operators.hpp>

namespace wuduo {
    // (1)Timestamp类继承自boost::less_than_comparable <T>模板类
    // 只要实现 <，即可自动实现>,<=,>=


class Timestamp : public muduo::copyable,
                  public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp>  {
public:
    Timestamp() : microSecondsSinceEpoch_(0) {}
    Timestamp(int64_t microSecondsSinceEpoch) : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}
    void swap(Timestamp &time) {
        std::swap(microSecondsSinceEpoch_, time.microSecondsSinceEpoch_);
    }

    string toString() const;
    string toFormattedString(bool showMicroSeconds = true) const;
    bool valid() const {
        return microSecondsSinceEpoch_ > 0;
    }
    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_;}
    time_t secondsSinceEpoch() const {
        return static_cast<time_t > (microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    }

    //获取当前时间
    static Timestamp now();

    //获取一个失效的时间，看第一个构造函数那里
    static Timestamp invalid() {
        return Timestamp();
    }

    static Timestamp fromUnixTime(time_t t) {
        return fromUnixTime(t, 0);
    }

    static Timestamp fromUnixTime(time_t t, int microseconds) {
        return Timestamp(static_cast<int64_t >(t) * kMicroSecondsPerSecond + microseconds);
    }

    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t microSecondsSinceEpoch_;

};

inline bool operator<(Timestamp &lhs, Timestamp &rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp &lhs, Timestamp &rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}
}



#endif //WUDUO_TIMESTAMP_H
