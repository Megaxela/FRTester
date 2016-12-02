//
// Created by megaxela on 02.12.16.
//

#include "TestDriverHolder.h"

TestDriver& TestDriverHolder::driver()
{
    static TestDriver driver;
    return driver;
}