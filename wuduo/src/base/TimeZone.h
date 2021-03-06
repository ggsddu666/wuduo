#ifndef WUDUO_TIMEZONE_H
#define WUDUO_TIMEZONE_H

#include "copyable.h"
#include <boost/shared_ptr.hpp>
#include <time.h>

namespace wuduo {
class TimeZone: public wuduo::copyable {
public:
    explicit TimeZone(const char *zonefile);
    TimeZone(int eastOfUtc, const char *tzname);
    TimeZone() {}

    bool valid() const {
        return static_cast<bool>(data_);
    }

    struct tm toLocalTime(time_t secondsSinceEpoch) const;
    time_t fromLocalTime(const struct tm&) const;

    static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);

    static time_t fromUtcTime(const struct tm&);

    static time_t fromUtcTime(int year, int month, int day, int hour, int minute, int seconds);

    struct Data;

private:
    boost::shared_ptr<Data> data_;
};
}


#endif