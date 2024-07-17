
#include "CommonTest.h"
#include "Logging.h"
#include <type_traits>

//折叠表达式（Fold Expressions）
template <typename... Args>
auto sum(Args... args)
{
    return (args + ...);  //折叠表达式
}

void checkIncrement()
{
    int value= 5;
    //value++ = 100;
    ++value = 100;

    LOG_INFO << "value : " << value;
}

void classOperatorIncrement()
{
    Value v(5);
    v.print(); // 输出 5
    ++v; // 调用前缀递增
    v.print(); // 输出 6
    v++; // 调用后缀递增
    v.print(); // 输出 7

    LOG_INFO << "value size : " << sizeof(Value);
}

void checkDecaytype()
{
    // 类型减弱
    static_assert(std::is_same<std::decay<int>::type, int>::value, "int should decay to int");
    static_assert(std::is_same<std::decay<int&>::type, int>::value, "int& should decay to int");
    static_assert(std::is_same<std::decay<int&&>::type, int>::value, "int&& should decay to int");
    static_assert(std::is_same<std::decay<const int&>::type, int>::value, "const int& should decay to int");
    static_assert(std::is_same<std::decay<int[5]>::type, int*>::value, "int[5] should decay to int*");
    static_assert(std::is_same<std::decay<void(int)>::type, void(*)(int)>::value, "void(int) should decay to void(*)(int)");
}

template<typename T, typename U>
auto add_me(T a, U b) {
    return a + b;
}

int fibonacci(int n) {
    // 基本情况
    if (n <= 1) {
        return n;
    }
    // 递归调用
    return fibonacci(n-1) + fibonacci(n-2);
}

class ParentA
{
public:
    ParentA(){};
    virtual ~ParentA(){};
    virtual void doSomethingA() = 0;
    virtual void doSomethingCommon() = 0;
};

class ParentB
{
public:
	ParentB(){};
	virtual ~ParentB(){};
	virtual void doSomethingB() = 0;
	virtual void doSomethingCommon() = 0;
};

class Inherit: public ParentA, public ParentB
{
public:
	Inherit(){};
	void doSomethingA(){};
	void doSomethingB(){};
	void doSomethingCommon(){};
};

//  逗号表达式
template<typename T, typename... Ts>
auto printf3(T value, Ts... args) {
    LOG_INFO << value;
    (void) std::initializer_list<T>{
        ([&args] {LOG_INFO << args;}(), value)...
    };
    // (void) std::initializer_list<int>{1,2,3,4,5,6}; (void)忽略该表达式的结果
}

template<typename T, typename... Ts>
auto printf4(T value, Ts... args) {
    LOG_INFO << value;
    (void) (([&args] {LOG_INFO << args;}()), ...);
}

template<typename T0, typename... T>
void printf5(T0 t0, T... t) {
    LOG_INFO << t0;
    if constexpr (sizeof...(t) > 0) printf5(t...);
}

bool runCommonTest()
{
    LOG_INFO << "CommonTest run test";

    // LOG_INFO <<  sum(1, 1.1, true);

    // checkIncrement();

    // classOperatorIncrement();

    // checktype();

    // LOG_INFO << add_me(3.1 , 4);

    // LOG_INFO << fibonacci(1);

    // printf3(111, 123, "alpha", 1.2);
    // printf4(111, 123, "alpha", 1.2);
    // printf5(111, 123, "alpha", 1.2);


    //c++17 新特性 continue

    return true;
}

namespace nm_CommonTestt{
      bool _=runCommonTest();
}