#ifndef WUDUO_DATE_H
#define WUDUO_DATE_H

#include "copyable.h"
#include "Types.h"

struct tm;

//  Julian Date: 是指由公元前4713年1月1日，协调世界时中午12时开始所经过的天数
namespace wuduo {
class Date: public wuduo::copyable {
public:
    struct YearMonthDay {
        int year;   // 1900~2500
        int month;  // 1~12
        int day;    // 1~31
    };

    static const int kDaysPerWeek = 7;
    static const int kJulianDayOf1970_01_01;

    Date(): julianDateNumber_(0) {}
    Date(int year, int month, int day);
    explicit Date(int julianDateNumber): julianDateNumber_(julianDateNumber) {}
    explicit Date(const struct tm&);

    void swap(Date &that) {
        std::swap(julianDateNumber_, that.julianDateNumber_);
    }
    bool valid() const { return julianDateNumber_ > 0; }
    std::string toIsoString() const;
    struct YearMonthDay yearMonthDay() const;
    int year() const {
        return yearMonthDay().year;
    }
    int month() const {
        return yearMonthDay().month;
    }
    int day() const {
        return yearMonthDay().day;
    }
    // 0~6  ->  Sunday, Monday...
    int weekDay() const {
        return (julianDateNumber_+1) % kDaysPerWeek;
    }
    int julianDayNumber() const {
        return julianDateNumber_;
    }

private:
    int julianDateNumber_;
};

inline bool operator<(Date x, Date y) {
    return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y) {
    return x.julianDayNumber() == y.julianDayNumber();
}
}

#endif