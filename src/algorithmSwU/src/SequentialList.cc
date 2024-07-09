#include "SequentialList.h"
#include "Logging.h"
#include <iostream>
#include <vector>

void SequentialList::testrun(void)
{
    LOG_INFO << "SequentialList run test";
}

bool runSequentialListTest()
{
    SequentialList s;
    s.testrun();

    std::tuple<int, double, std::string> myTuple = {1, 2.2, "three"};

    LOG_INFO << myTuple;

    return true;
}

namespace nm_SequentialListTest{
    bool _ = runSequentialListTest();
}
