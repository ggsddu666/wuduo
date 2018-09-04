#include <stdio.h>
#include <inttypes.h>
// #include "base/Types.h"
#include "base/FileUtil.h"

using namespace wuduo;


int main() {
    std::string result;
    int64_t size = 0;
    //    21 0 0
    //    21 0 0
    int err = FileUtil::readFile("/proc/self", 1024, &result, &size);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);
    err = FileUtil::readFile("/proc/self", 1024, &result, NULL);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);

    //    0 10 0
    //    2 10 0
    err = FileUtil::readFile("/proc/self/cmdline", 1024, &result, &size);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);
    err = FileUtil::readFile("/proc/null", 1024, &result, &size);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);
    // err = FileUtil::readFile("")

    //    0 1024 0
    //    2 1024 0
    err = FileUtil::readFile("/dev/zero", 1024, &result, &size);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);
    err = FileUtil::readFile("/noteexist", 1024, &result, &size);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);

    //    0 102400 0
    //    0 102400 0
    err = FileUtil::readFile("/dev/zero", 102400, &result, &size);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);
    err = FileUtil::readFile("/dev/zero", 102400, &result, NULL);
    printf("%d %zd %" PRIu64 "\n", err, result.size(), size);
    return 0;
}