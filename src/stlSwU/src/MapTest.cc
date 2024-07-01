#include "MapTest.h"
#include <iostream>


void MapTest::testrun(void)
{
    std::cout<< "Map run test" <<std::endl;
}

bool runMaptest()
{
    MapTest m;
    m.testrun();
    return true;
}

namespace nm_MapTest{
    bool _=runMaptest();
}