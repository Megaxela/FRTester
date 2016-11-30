//
// Created by megaxela on 17.11.16.
//

#include "include/DriverHolder.h"


FRDriver& DriverHolder::driver()
{
    static FRDriver driver;

    return driver;
}