//
// Created by megaxela on 15.12.16.
//

#include "include/Tools/StdExtend.h"

STDEXTEND_NAMESPACE
{
    bool begins(const std::string &s, const std::string &with)
    {
        return s.compare(0, with.size(), with) == 0;
    }
};