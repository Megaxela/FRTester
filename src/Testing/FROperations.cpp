//
// Created by megaxela on 10.01.17.
//

#include "Testing/FROperations.h"
#include <cmath>

uint32_t FROperations::smartRound(double val)
{
    if ((val - (int) val) < 0.5)
    {
        return (uint32_t) std::floor(val);
    }
    else
    {
        return (uint32_t) std::ceil(val);
    }
}