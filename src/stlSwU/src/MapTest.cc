#include "MapTest.h"
#include "Logging.h"
#include <iostream>

void MapTest::testrun(void)
{
    LOG_INFO << "Map run test";
}

bool runMaptest()
{
    MapTest m;
    m.testrun();
    return true;
}

namespace nm_MapTest{
    //bool _=runMaptest();
}