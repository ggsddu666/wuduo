//
// Created by wu on 2018/8/19.
//

#ifndef WUDUO_TYPE_H
#define WUDUO_TYPE_H

#include <stdint.h>
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

namespace muduo {
    using std::string;

    // 类型强转，implicit_cast只能执行up-cast，派生类->基类
    // 这里模拟了boost库里面的implicit_cast
    template <typename To, typename From>
    inline To implicit_cast(const From &f) {
        return f;
    };

    //进行down_cast 基类->派生类转换，并检查转换是否安全
    template <typename To, typename From>
    inline To down_cast(From *f) {

        // if (false)
        //  {
        //    implicit_cast<From*, To>(0);
        //  }

        // static_cast与dynamic_cast:
        // 前者提供的是编译时期的静态类型检测,后者提供的是运行时检测.
        // dynamic_cast转换如果成功的话返回的是指向类的指针或引用，转换失败的话则会返回NULL。
        // 在类的转换时，在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的。
        // 在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。

        // RTTI: debug mode only!
        // dynamic_cast是4个转换中唯一的RTTI操作符，提供运行时类型检查。
        #if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RIIT)
            assert(f == NULL || dynamic_cast<To>(f) != NULL);
        #endif

        return static_cast<To>(f);
    };
}


#endif //WUDUO_TYPE_H
