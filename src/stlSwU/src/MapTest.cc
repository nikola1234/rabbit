#include "MapTest.h"
#include "Logging.h"
#include <iostream>
#include <map>
#include <unordered_map>

void MapTest::testrun(void)
{
    LOG_INFO << "Map run test";
/*
    std::unordered_map<std::string,std::string> mymap;

    mymap["Bakery"]="Barbara";  // new element inserted
    mymap["Seafood"]="Lisa";    // new element inserted
    mymap["Produce"]="John";    // new element inserted

    std::string name = mymap["Bakery"];   // existing element accessed (read)
    mymap["Seafood"] = name;              // existing element accessed (written)

    LOG_INFO << mymap["Seafood"];

    mymap["Bakery"] = mymap["Produce"];   // existing elements accessed (read/written)

    name = mymap["Deli"];      // non-existing element: new element "Deli" inserted!

    mymap["Produce"] = mymap["Gifts"];    // new element "Gifts" inserted, "Produce" written

    for (auto& x: mymap) {
        LOG_INFO <<  x.first << ": " << x.second;
    }
*/

    std::multimap<std::string, int> myMultimap;

    // 插入元素
    myMultimap.insert(std::make_pair("apple", 5));
    myMultimap.insert(std::make_pair("banana", 10));
    myMultimap.insert(std::make_pair("apple", 3)); // 允许键重复
    myMultimap.insert(std::make_pair("apple", 6)); // 允许键重复

    // 遍历并打印元素
    for (const auto& kv : myMultimap) {
        LOG_INFO << kv.first << ": " << kv.second;
    }

    // 查找并打印所有键为 "apple" 的元素
    auto range = myMultimap.equal_range("apple");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << "apple: " << it->second << std::endl;
    }

}

bool runMaptest()
{
    MapTest m;
    m.testrun();
    return true;
}

namespace nm_MapTest{
     // bool _=runMaptest();
}