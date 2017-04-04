//
// Created by megaxela on 10.01.17.
//

#include "Testing/FROperations.h"
#include <cmath>
#include <shared_libs/frdrvcross/include/Tools/Logger.h>

uint32_t FROperations::smartRound(double val)
{
    ExcessLogStreamEx("FROperations") << "Число: " << val << ", дробная часть: " << val - (int) val << std::endl;
    if ((val - (int) val) < 0.5)
    {
        return (uint32_t) std::floor(val);
    }
    else
    {
        return (uint32_t) std::ceil(val);
    }
}

uint64_t FROperations::multiply(uint64_t price, uint64_t count)
{
    ExcessLogStreamEx("FROperations") << "Умножение (стоимость на количество): " << price << " * " << count << std::endl;
    return smartRound((price * count) / 1000.0);
}
