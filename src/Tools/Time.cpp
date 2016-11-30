//
// Created by megaxela on 14.11.16.
//

#include "Tools/Time.h"

Time::time_t Time::getMilliseconds()
{
    return std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

Time::time_t Time::getMicroseconds()
{
    return std::chrono::duration_cast< std::chrono::microseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}