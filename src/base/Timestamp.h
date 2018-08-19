//
// Created by wu on 2018/8/19.
//

#ifndef WUDUO_TIMESTAMP_H
#define WUDUO_TIMESTAMP_H

#include "copyable.h"

namespace wuduo {
    // (1)Timestamp类继承自boost::less_than_comparable <T>模板类
    // 只要实现 <，即可自动实现>,<=,>=


class Timestamp : public muduo::copyable, public boost::{

};
}



#endif //WUDUO_TIMESTAMP_H
