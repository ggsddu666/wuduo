#include "TimeZone.h"
#include "Date.h"
#include <boost/noncopyable.hpp>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace wuduo {
namespace detail {
struct Transition {
    time_t gmttime;
    time_t localtime;
    int localtimeIdx;

    Transition(time_t t, time_t l, int localIdx): gmttime(t), localtime(l), localtimeIdx(localIdx) {

    }
};

struct Comp {
    bool compareGmt;

    Comp(bool gmt): compareGmt(gmt) {}

    // 函数调用运算符 () 可以被重载用于类的对象。
    // 当重载 () 时，您不是创造了一种新的调用函数的方式，
    // 相反地，这是创建一个可以传递任意数目参数的运算符函数。
    // http://www.runoob.com/cplusplus/function-call-operator-overloading.html
    bool operator()(const Transition &lhs, const Transition &rhs) const {
        if(compareGmt)
            return lhs.gmttime < rhs.gmttime;
        else
            return lhs.localtime < rhs.localtime;
    }

    bool equal(const Transition &lhs, const Transition &rhs) const {
        if(compareGmt)
            return lhs.gmttime == rhs.gmttime;
        else
            return lhs.localtime == lhs.localtime;
    }
};

struct Localtime {
    time_t gmtOffset;
    bool isDst;
    int arrbIdx;

    Localtime(time_t offset, bool dst, int arrb): gmtOffset(offset), isDst(dst), arrbIdx(arrb) {

    }
};

inline void fillHMS(unsigned seconds, struct tm *utc) {
    utc->tm_sec = seconds % 60;
    unsigned minutes = seconds / 60;
    utc->tm_min = minutes % 60;
    utc->tm_hour = minutes / 60;
}

}

const int kSecondsPerDay = 24*60*60;

}

using namespace wuduo;
using namespace std;

struct TimeZone::Data {
    vector<detail::Transition> transitions;
    vector<detail::Localtime> localtimes;
    vector<string> names;
    string abbreviation;
};

namespace wuduo {
namespace detail {
class File: boost::noncopyable {
public:
    File(const char *file): fp_(::fopen(file, "rb")) {}
    ~File() {
        if(fp_) {
            ::fclose(fp_);
        }
    }

    bool valid() const { return fp_; }

    string readBytes(int n) {
        char buf[n];
        // size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
        //        ptr -- 这是指向带有最小尺寸 size*nmemb 字节的内存块的指针。
        //        size -- 这是要读取的每个元素的大小，以字节为单位。
        //        nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
        //        stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
        ssize_t nr = fread(buf, 1, n, fp_);
        if(nr != n)
            throw logic_error("no enough data");
        // string s(chars,chars_len)
        // 将C字符串前chars_len个字符作为字符串s的初值。
        return string(buf, n);
    }

    int32_t readInt32() {
        int32_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(int32_t), fp_);
        if(nr != sizeof(int32_t))
            throw logic_error("bad int32_t data");
        // 名称为“be nn toh” 的函数从big-endian（大端）顺序转换为主机字节顺序。
        return be32toh(x);
    }

    uint8_t readUInt8() {
        uint8_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), fp_);
        if(nr != sizeof(uint8_t))
            throw logic_error("bad int8_t data");
        return x;
    }

private:
    FILE *fp_;
};

// 以/usr/share/zoneinfo/America/New_York为例
bool readTimeZoneFile(const char *zonefile, struct TimeZone::Data *data) {
    File f(zonefile);
    if(f.valid()) {
        try {
            // head: "TZif"
            string head = f.readBytes(4);
            if(head != "TZif")
                throw logic_error("bad head");
            // version: 2
            string version = f.readBytes(1);

            // f.readBytes(15): (空格)  如果去掉，下面读取会有问题
            f.readBytes(15);

            // 下面六个值
            // 55 5 0
            // 2365 5 20
            int32_t isgmtcnt = f.readInt32();
            int32_t isstdcnt = f.readInt32();
            int32_t leapcnt = f.readInt32();
            int32_t timecnt = f.readInt32();
            int32_t typecnt = f.readInt32();
            int32_t charcnt = f.readInt32();

//            std::cout << isgmtcnt << isgmtcnt << " " << isstdcnt << " " << leapcnt <<std::endl;
//            std::cout << timecnt << isgmtcnt << " " << typecnt << " " << charcnt <<std::endl;
//            exit(EXIT_SUCCESS);

            vector<int32_t > trans;
            vector<int> localtimes;
            // timecnt: 2365
            trans.reserve(timecnt);

            // -2147483648
            // -1633280400
            // -1615140000
            // -1601830800
            // ...
            // ...
            // 2088658800
            // 2109218400
            // 2120108400
            // 2140668000
            for(int i = 0; i < timecnt; ++i) {
               // std::cout << f.readInt32() << std::endl;
                trans.push_back(f.readInt32());
            }

            // local为空？？？？？？
            for(int i = 0; i < timecnt; ++i) {
                uint8_t local = f.readUInt8();
                // localtimes存的是index
                localtimes.push_back(local);
                // std::cout << local << std::endl;
            }

            // exit(EXIT_SUCCESS);

            for(int i = 0; i < typecnt; ++i) {
                int32_t gmtoff = f.readInt32();
                uint8_t isdst = f.readUInt8();
                uint8_t abbrind = f.readUInt8();

                // data是函数参数
                data->localtimes.push_back(Localtime(gmtoff, isdst, abbrind));
            }

            for(int i = 0; i < timecnt; ++i) {
                int localIdx = localtimes[i];
                time_t localtime = trans[i] + data->localtimes[localIdx].gmtOffset;
                data->transitions.push_back(Transition(trans[i], localtime, localIdx));
            }

            data->abbreviation = f.readBytes(charcnt);

            for(int i = 0; i < leapcnt; ++i) {

            }

            // ???
            (void) isstdcnt;
            (void) isgmtcnt;

        } catch (logic_error &e) {
            fprintf(stderr, "%s\n", e.what());
        }

    }

    return true;
}

const Localtime* findLocaltime(const TimeZone::Data &data, Transition sentry, Comp comp) {
    const Localtime *local = NULL;

    // local指向
    if(data.transitions.empty() || comp(sentry, data.transitions.front())) {
        local = &data.localtimes.front();
    } else {
        // lower_bound()返回一个 iterator 它指向在[first,last)标记的有序序列中可以插入value，
        // 而不会破坏容器顺序的第一个位置，而这个位置标记了一个不小于value 的值
        vector<Transition>::const_iterator transI = lower_bound(data.transitions.begin(),
                                                                data.transitions.end(),
                                                                sentry,
                                                                comp);
        if(transI != data.transitions.end()) {
            if(!comp.equal(sentry, *transI)) {
                assert(transI != data.transitions.begin());
                --transI;
            }
            local = &data.localtimes[transI->localtimeIdx];
        } else {
            local = &data.localtimes[data.transitions.back().localtimeIdx];
        }
    }

    return local;
}


}
}

TimeZone::TimeZone(const char *zonefile): data_(new TimeZone::Data) {
    if(!detail::readTimeZoneFile(zonefile, data_.get())) {
        data_.reset();
    }
}

TimeZone::TimeZone(int eastOfUtc, const char *name): data_(new TimeZone::Data) {
    data_->localtimes.push_back(detail::Localtime(eastOfUtc, false, 0));
    data_->abbreviation = name;
}

struct tm TimeZone::toLocalTime(time_t seconds) const {
    struct tm localTime;
    bzero(&localTime, sizeof(localTime));
    assert(data_ != NULL);
    const Data &data(*data_);

    detail::Transition sentry(seconds, 0, 0);
    const detail::Localtime *local = findLocaltime(data, sentry, detail::Comp(true));

    if(local) {
        time_t localSeconds = seconds + local->gmtOffset;
        // gmtime() 函数将日历时间timep（秒数）转换为用UTC时间表示的时间（年月日时分秒）。
        // 它可能返回NULL，比如年份不能放到一个整数中。
        // 返回值指向一个静态分配的结构，该结构可能会被接下来的任何日期和时间函数调用覆盖。
        // gmtime_r()函数功能与此相同，但是它可以将数据存储到用户提供的结构体中。

        // UTC时间（unix时戳均使用1970年1月1日0时0分0秒到目前的UTC秒数）
        // UTC时间是UNIX系统所使用的标准时间，
        // 不同地区的人使用时显示的本地时间由UTC+时区而来（也可以说所有Unix/linux操作系统的系统时间都相同，1970年1月1日0时0分0秒到目前的UTC秒数）。
        ::gmtime_r(&localSeconds, &localTime);
        localTime.tm_isdst = local->isDst;
        localTime.tm_gmtoff = local->gmtOffset;
        localTime.tm_zone = &data.abbreviation[local->arrbIdx];
    }

    return localTime;
}

time_t TimeZone::fromLocalTime(const struct tm &localTm) const {
    assert(data_ != NULL);
    const Data &data(*data_);

    struct tm tmp = localTm;
    // 从分解时间tm（被视作UTC时间，不考虑本地时区设置）到日历时间time_t的转换。
    time_t seconds = ::timegm(&tmp);
    detail::Transition sentry(0, seconds, 0);
    // comp(false)  比较的是sentry第二个参数（seconds），也就是localtime
    const detail::Localtime *local = findLocaltime(data, sentry, detail::Comp(false));
    if (localTm.tm_isdst)
    {
        struct tm tryTm = toLocalTime(seconds - local->gmtOffset);
        if (!tryTm.tm_isdst && tryTm.tm_hour == localTm.tm_hour && tryTm.tm_min == localTm.tm_min) {
            seconds -= 3600;
        }
    }
    return seconds - local->gmtOffset;
}

// static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);

struct tm TimeZone::toUtcTime(time_t secondsSinceEpoch, bool yday)
{
    struct tm utc;
    bzero(&utc, sizeof(utc));
    utc.tm_zone = "GMT";
    int seconds = static_cast<int>(secondsSinceEpoch % kSecondsPerDay);
    int days = static_cast<int>(secondsSinceEpoch / kSecondsPerDay);
    if (seconds < 0)
    {
        seconds += kSecondsPerDay;
        --days;
    }
    detail::fillHMS(seconds, &utc);
    Date date(days + Date::kJulianDayOf1970_01_01);
    Date::YearMonthDay ymd = date.yearMonthDay();
    utc.tm_year = ymd.year - 1900;
    utc.tm_mon = ymd.month - 1;
    utc.tm_mday = ymd.day;
    utc.tm_wday = date.weekDay();

    if (yday)
    {
        Date startOfYear(ymd.year, 1, 1);
        utc.tm_yday = date.julianDayNumber() - startOfYear.julianDayNumber();
    }
    return utc;
}

time_t TimeZone::fromUtcTime(const struct tm& utc) {
    return fromUtcTime(utc.tm_year + 1900, utc.tm_mon + 1, utc.tm_mday, utc.tm_hour, utc.tm_min, utc.tm_sec);
}


//
time_t TimeZone::fromUtcTime(int year, int month, int day, int hour, int minute, int seconds) {
    Date date(year, month, day);
    int secondsInDay = hour * 3600 + minute * 60 + seconds;
    time_t days = date.julianDayNumber() - Date::kJulianDayOf1970_01_01;
    return days * kSecondsPerDay + secondsInDay;
}


