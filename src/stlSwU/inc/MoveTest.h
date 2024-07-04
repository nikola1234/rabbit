#pragma once

#include "Logging.h"
#include <iostream>
#include <utility> // 包含 std::move


class MoveTest2
{
public:
    int data = 2;
    MoveTest2() = default;

    MoveTest2(MoveTest2&& other){
        LOG_INFO << "Move Constructor";
    }

    void setData(int d) {data =d;}
    int getData() {return data;}

    // 禁用拷贝构造函数和拷贝赋值运算符（可选）
    MoveTest2(const MoveTest2&) = delete;
    MoveTest2& operator=(const MoveTest2&) = delete;
};

class MoveTest
{
public:
    int* data;

    // 默认构造函数
    MoveTest() : data(new int(10)) {
        LOG_INFO << "Default Constructor";
    }

    // 移动构造函数
    MoveTest(MoveTest&& other) noexcept : data(other.data) {
        other.data = nullptr; // 避免删除同一块内存
        LOG_INFO << "Move Constructor";
    }

    // 析构函数
    ~MoveTest() {
        if (data != nullptr)
        {
            LOG_INFO << "not nullptr : " << *data;
            delete data;
        }
        LOG_INFO << "Destructor";
    }

    // 移动赋值运算符
    MoveTest& operator=(MoveTest&& other) noexcept {
        LOG_INFO << "Move assignment operator";
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    // 禁用拷贝构造函数和拷贝赋值运算符（可选）
    MoveTest(const MoveTest&) = delete;
    MoveTest& operator=(const MoveTest&) = delete;
};