//
// Created by wu on 2018/8/20.
//

#ifndef WUDUO_TIMESTAMP_H
#define WUDUO_TIMESTAMP_H

// 解释1：
//C++里面有string和char*，如果你用const string &s 做函数形参，
// 可以同时兼容两种字符串。但当你传入一个很长的char * 时，
// 会生成一个较大的string对象，开销比较大。

// 如果你的目的仅仅是读取字符串的值，用这个StringPiece的话，
// 仅仅是4+一个指针的内存开销，而且也保证了兼容性。

// 所以这个类的目的是传入字符串的字面值，
// 它内部的ptr_ 这块内存不归他所有。所以不能做任何改动。

// 解释2：
// 这个类实际上只是对字符串的一个proxy类而已（即设计模式中的代理模式），
// 通常叫做string view.它提供了一个窗口，外部仅可以观察到这个窗口中字符串的内容，
// 在调整窗口大小时不需要修改原字符串，仅移动开始指针和调整长度即可。
// 另外这个类自身并不存储这个字符串，所以它的有效生存期取决于源字符串指针的生存期。

#include <string.h>
#include <string>


namespace wuduo {
class StringArg {
public:
    StringArg(const char *str) : str_(str) {}
    StringArg(const std::string str) : str_(str.c_str()) { }
    const char *c_str() const { return str_; }


private:
    const char *str_;
};

// 字符串类不以Null为结束符（而是通过length），若是要以NUll为结束，则使用as_string().c_str()
class StringPiece {
private:
    const char *ptr_;
    int length_;

public:
    StringPiece() : ptr_(NULL), length_(0) {}
    StringPiece(const char *str) : ptr_(str), length_(static_cast<int>(strlen(ptr_))) {}
    StringPiece(const unsigned char *str) : ptr_(reinterpret_cast<const char *>(str)), length_(static_cast<int>(strlen(ptr_))) {}
//      StringPiece(const unsigned char* str)
//    : ptr_(reinterpret_cast<const char*>(str)),
//      length_(static_cast<int>(strlen(ptr_))) { }

    StringPiece(const std::string &str) : ptr_(str.data()), length_(static_cast<int>(str.size())) {}
    StringPiece(const char *offset, int len) : ptr_(offset), length_(len) {}

    const char *data() const { return ptr_;}
    int size() const { return length_; }
    bool empty() const { return length_ == 0; }
    const char *begin() const { return ptr_; }
    const char *end() const { return ptr_ + length_; }
    void clear() { ptr_ = NULL; length_ = 0; }
    void set(const char *buffer, int len) { ptr_ = buffer; length_ = len; }
    void set(const char * str) {
        ptr_ = str;
        length_ = static_cast<int>(strlen(str));
    }
    void set(const void *buffer, int len) {
        ptr_ = reinterpret_cast<const char*>(buffer);
        length_ = len;
    }

    char operator[](int i) const { return ptr_[i]; }

    void remove_prefix(int n) {
        ptr_ += n;
        length_ -= n;
    }

    void remove_suffix(int n) {
        length_ -= n;
    }

    bool operator==(const StringPiece &x) const {
        // C 库函数 int memcmp(const void *str1, const void *str2, size_t n))
        // 把存储区 str1 和存储区 str2 的前 n 个字节进行比较。
        return ((length_ == x.length_) && (memcmp(ptr_, x.ptr_, length_) == 0));
    }


    bool operator!=(const StringPiece &x) const {
        return !(*this == x);
    }

    //  r == 0
#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)       \
    bool operator cmp(const StringPiece &x) const {      \
        int r = memcmp(ptr_, x.ptr_, length_ < x.size() ? length_ : x.size());  \
        return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.size())));          \
    }

    STRINGPIECE_BINARY_PREDICATE(<, <);
    STRINGPIECE_BINARY_PREDICATE(<=, <);
    STRINGPIECE_BINARY_PREDICATE(>=, >);
    STRINGPIECE_BINARY_PREDICATE(>, >);
#undef STRINGPIECE_BINARY_PREDICATE


    int compare(const StringPiece &x) const {
        int r = memcmp(ptr_, x.ptr_, length_ < x.size() ? length_ : x.size());
        if(r == 0) {
            if(length_ < x.size()) r = -1;
            else if(length_ > x.size()) r = 1;
        }
        return r;
    }

    std::string as_string() const {
        return std::string(data(), size());
    }

    void CopyToString(std::string *target) const {
        // string &assign(const string &s,int start,int n);　　
        // 把字符串s中从start开始的n个字符赋给当前字符串
        target->assign(ptr_, length_);
    }

    // 判断this的开头是不是x字符串
    bool starts_with(const StringPiece &x) const {
        return ((length_ >= x.size()) && (memcmp(ptr_, x.ptr_, x.size()) == 0));
    }

};
}

// 萃取技术（用了特例化）https://blog.csdn.net/yusiguyuan/article/details/38866193
// 可以提高效率
#ifdef HAVE_TYPE_TRAITS
template<> struct __type_traits<wuduo::StringPiece> {
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
      typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};
#endif

std::ostream& operator<<(std::ostream &o, const wuduo::StringPiece &piece);




#endif




