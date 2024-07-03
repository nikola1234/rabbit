#include "SequentialList.h"
#include "Logging.h"
#include <iostream>

void SequentialList::testrun(void)
{
    LOG_INFO << "SequentialList run test";
}

bool runSequentialListTest()
{
    SequentialList s;
    s.testrun();
    return true;
}

namespace nm_SequentialListTest{
    //bool _ = runSequentialListTest();
}
