#include "ObjConstructor.h"
#include <iostream>


bool runObjConstructortest()
{
    LOG_INFO << "ObjConstructor run test";

    ObjConstructor obj1(10);
    ObjConstructor obj2(obj1);
    ObjConstructor obj3=obj1;

    ObjConstructor obj4;
    obj4=obj1;

    return true;
}

namespace nm_ObjConstructorTest{
    //bool _=runObjConstructortest();
}