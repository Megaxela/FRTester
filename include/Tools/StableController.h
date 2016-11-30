//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_STABLECONTROLLER_H
#define FRTESTER_STABLECONTROLLER_H

#include <functional>

namespace StableController
{
    void init(std::function<void()> cb);

    void excessExecute();

    static std::function<void()> _cb;
};

#endif //FRTESTER_STABLECONTROLLER_H
