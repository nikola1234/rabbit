#include "MoveTest.h"
#include <utility> // 包含 std::forward

void process(int& lval) {
    LOG_INFO << "Lvalue";
}

void process(int&& rval) {
    LOG_INFO << "Rvalue";
}

template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg)); // 完美转发 arg
}

bool runMoveTest()
{
    LOG_INFO << "MoveTest run test";

    // MoveTest obj1;
    // MoveTest obj2 = std::move(obj1); // 使用移动构造函数
    // 此时 obj1 的 data 成员已被设置为 nullptr

/*
    MoveTest2 mt_obj1;
    mt_obj1.setData(10);
    MoveTest2 mt_obj2 = std::move(mt_obj1); // 使用移动构造函数

    LOG_INFO << mt_obj1.getData();
    LOG_INFO << mt_obj2.getData();

*/
    int a = 10;
    wrapper(a);    // 调用 process(int&)
    wrapper(20);   // 调用 process(int&&)

    return true;
}

namespace nm_MoveTest{
    //  bool _=runMoveTest();
}