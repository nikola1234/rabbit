#pragma once

#include "Logging.h"

class ObjConstructor
{
public:

    ObjConstructor() {ptr = nullptr;}

    ~ObjConstructor() {delete ptr;}

    ObjConstructor(int value)
    {
        ptr = new int(value);
    }

    // copy constructor
    ObjConstructor(const ObjConstructor& other)  // 创建新的对象
    {
        LOG_INFO << "enter copy constructor";
        ptr = new int(*other.ptr);
    }

    // assignment constructor
    ObjConstructor& operator=(const ObjConstructor& other)
    {
        LOG_INFO << "enter assignment constructor";
        if (this != &other)
        {
            delete ptr;
            ptr = new int(*other.ptr);
        }
        return *this;
    }

private:
    int* ptr;
};