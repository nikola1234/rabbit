
#include "CommonTest.h"
#include "Logging.h"


//折叠表达式（Fold Expressions）
template <typename... Args>
auto sum(Args... args)
{
    return (args + ...);  //折叠表达式
}

bool runCommonTest()
{
    LOG_INFO << "CommonTest run test";

    // LOG_INFO <<  sum(1, 1.1, true);

    //c++17 新特性 continue

    return true;
}

namespace nm_CommonTestt{
    bool _=runCommonTest();
}