#include "Exception.h"

#include <execinfo.h>
#include <stdlib.h>

using namespace wuduo;

Exception::Exception(const char *msg): message_(msg) {
    fillStackTrace();
}

Exception::Exception(const string &msg): message_(msg) {
    fillStackTrace();
}

Exception::~Exception() throw() {

}

// 它是函数提供者和使用者的一种约定，即提供者告诉使用者。此函数会抛出哪个异常。
// 如果，我们函数已经标明为 throw()
// 那么你 try{}catch(…) 是不起作用的，程序直接 abort。
const char* Exception::what() const throw() {
    return message_.c_str();
}

const char* Exception::stackTrace() const throw() {
    return stack_.c_str();
}

void Exception::fillStackTrace() {
    const int len = 200;
    void *buffer[len];

    // backtrace()函数用来获取程序中当前函数的回溯信息,
    // 即一系列的函数调用关系，获取到的信息被放在参数buffer中。

    // buffer是一个数组指针，数组的每个元素保存着每一级被调用函数的返回地址。
    // 参数size指定了buffer中可存放的返回地址的数量。如果函数实际的回溯层级数大于size，
    // 则buffer中只能存放最近的函数调用关系，所以，想要得到完整的回溯信息，就要确保size参数足够大。

    // backtrace()函数的返回值为buffer中的条目数量，这个值不一定等于size，因为如果为得到完整回溯信息而将size设置的足够大，则该函数的返回值为buffer中实际得到的返回地址数量。
    int nptrs = ::backtrace(buffer, len);

    // 通过backtrace()函数得到buffer之后，backtrace_symbols()可以将其中的返回地址都对应到具体的函数名，参数size为buffer中的条目数。
    // backtrace_symbols()函数可以将每一个返回值都翻译成“函数名+函数内偏移量+函数返回值”，这样就可以更直观的获得函数的调用关系。
    // 经过翻译后的函数回溯信息放到backtrace_symbols()的返回值中，如果失败则返回NULL。
    char **strings = ::backtrace_symbols(buffer, nptrs);
    if(strings) {
        for(int i = 0; i < nptrs; ++i) {
            stack_.append(strings[i]);
            stack_.push_back('\n');
        }

        // 需要注意，返回值本身是在backtrace_symbols()函数内部进行malloc的，所以必须在后续显式地free掉。
        free(strings);
    }

}