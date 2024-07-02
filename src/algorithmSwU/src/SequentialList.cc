#include "SequentialList.h"
#include "logging.h"


void SequentialList::testrun(void)
{
    logger l;
    l.output("SequentialList run test");
}

bool runSequentialListTest()
{
    SequentialList s;
    s.testrun();
    return true;
}

namespace nm_SequentialListTest{
    bool _ = runSequentialListTest();
}
