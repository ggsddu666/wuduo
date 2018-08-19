//
// Created by wu on 2018/8/19.
//

#ifndef WUDUO_COPYABLE_H
#define WUDUO_COPYABLE_H

namespace muduo
{

    // 这是一个空类。会被用作其他类的基类。正如其名字一样，这个基类是可以被copy的。
    // 在《Effective C++》条款6中介绍了如何声明Uncopyable的类。

    class copyable
    {
    };

};

#endif //WUDUO_COPYABLE_H
