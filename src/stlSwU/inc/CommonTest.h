#pragma once
#include "Logging.h"

class Value
{
private:
    int value_;
    char c;

public:
    // 构造函数
    Value(int value = 0) : value_(value) {}

    // 前缀递增
    Value& operator++() {
        LOG_INFO << "前缀递增";
        ++value_; // 增加内部值
        return *this; // 返回自身的引用，允许链式调用
    }

    // 后缀递增
    Value operator++(int dummy) {
        LOG_INFO << "后缀递增 " << dummy ;
        Value temp = *this; // 保存当前值以便返回
        ++(*this); // 调用前缀递增来增加内部值
        return temp; // 返回增加之前的值   **后缀递增需要返回修改之前的值，
    }

    // 为了展示效果，添加一个函数来输出Value的值
    void print() const {
        LOG_INFO << value_;
    }
};

class CommonTest
{
    public:

};
