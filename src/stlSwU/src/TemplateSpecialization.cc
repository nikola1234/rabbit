#include "TemplateSpecialization.h"
#include "Logging.h"
namespace{

/*
当通用模板无法适用于特定类型或需要特殊处理时，可以使用模板特化来为这些特定类型提供专门的实现。这样，编译器在编译时会根据特定的类型选择相应的特化版本进行实例化和编译。

模板特化有两种形式：完全特化（Full specialization）和偏特化（Partial specialization）。

完全特化（Full specialization）：为某个特定类型提供独立的实现。完全特化的模板定义在通用模板定义后，使用template <>开头，
                                并包括特定类型的参数列表。完全特化的模板提供了对特定类型的精确匹配，并在特化版本中指定了具体的实现。

偏特化（Partial specialization）：为一类相关类型提供通用的实现。偏特化的模板定义在通用模板定义后，使用template <...>开头，
                                并包括特定类型的参数列表和模板参数列表。偏特化的模板提供了对特定类型属性的匹配，并在特化版本中指定了通用的实现。

模板特化允许编写更专门和具体的代码，根据特定类型的需要进行定制。它在使用STL（标准模板库）和元编程时特别有用，可以为不同类型提供不同的行为。
*/

//######################### CheckVectorType1 ####################################
template<typename T>
struct is_vector : std::false_type {};

template<typename... Args>
struct is_vector<std::vector<Args...>> : std::true_type {};

template<typename T>
void CheckVectorType1(const T& obj) {
    if constexpr (is_vector<T>::value) {
        using ValueType = typename T::value_type;
        LOG_INFO << "Type is std::vector<" << typeid(ValueType).name() << ">";
    } else {
        LOG_INFO << "Type is not std::vector";
    }
}

//######################### CheckVectorType2 ####################################
template <typename T>
void CheckVectorType2(const T& obj)
{
    if constexpr (std::is_same<T, std::vector<typename T::value_type, typename T::allocator_type>>::value)
    {
        using ValueType = typename T::value_type;
        LOG_INFO << "Type is std::vector<" << typeid(ValueType).name() << ">";
    }
    else
    {
        LOG_INFO << "Type is not std::vector";
    }
}

//######################### CheckVectorType3 ####################################

template<typename T>
typename std::enable_if_t<std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>>>
CheckVectorType3(const T& obj)
{
    using ValueType = typename T::value_type;
    LOG_INFO << "Type is std::vector<" << typeid(ValueType).name() << ">";
}

template<typename T>
typename std::enable_if_t<!std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>>>
CheckVectorType3(const T& obj)
{
    LOG_INFO << "Type is not std::vector";
}


//######################### Full specialization ####################################
template <typename T>
void processFull()
{
    LOG_INFO << "common proess";
}

template <>
void processFull<int>()
{
    LOG_INFO << "int proess";
}

//######################### Partial specialization 偏特化是模板类的特性####################################
// 定义一个通用的模板类
template <typename T, typename N>
class ProcessPartial {
public:
    void operator()(T t, N n) const {
        LOG_INFO << "common Partial process with T=" << t << " and N=" << n;
    }
};

// 偏特化，用于当T是int的时候
template <typename N>
class ProcessPartial<int, N> {
public:
    void operator()(int t, N n) const {
        LOG_INFO << "int Partial process with T=" << t << " and N=" << n;
    }
};

}

bool runTemplateSpecializationtest()
{
    LOG_INFO << "TemplateSpecialization run test";

    std::vector<int> intVector;
    std::vector<double> doubleVector;
    std::vector<std::string> stringVector;
    std::string myString;

    CheckVectorType1(intVector);
    CheckVectorType1(doubleVector);
    CheckVectorType1(stringVector);
    CheckVectorType1(myString);

    CheckVectorType2(intVector);
    CheckVectorType2(doubleVector);
    CheckVectorType2(stringVector);
    CheckVectorType2(myString);

    CheckVectorType3(intVector);
    CheckVectorType3(doubleVector);
    CheckVectorType3(stringVector);
    CheckVectorType3(myString);

    processFull<double>();
    processFull<int>();

    ProcessPartial<int, float> pf;
    pf(10, 20.5f); // 将使用特化版本

    ProcessPartial<std::string, float> sf;
    sf("Hello", 20.5f); // 将使用通用版本
    return true;
}

namespace nm_TemplateSpecializationTest{
    // bool _=runTemplateSpecializationtest();
}