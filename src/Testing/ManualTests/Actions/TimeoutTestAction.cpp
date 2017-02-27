//
// Created by megaxela on 22.02.17.
//

#include <include/Tools/Time.h>
#include "include/Testing/ManualTests/Actions/TimeoutTestAction.h"

TimeoutTestAction::TimeoutTestAction() :
    AbstractTestAction("Таймаут",
                       "Таймаут в мс.",
                       {{"Таймаут (в мс)", (uint32_t) 1000}})
{

}

TimeoutTestAction::~TimeoutTestAction()
{

}

bool TimeoutTestAction::execute()
{
    Time::sleep<std::chrono::milliseconds>(
            getValue("Таймаут (в мс)").toUInt32()
    );

    return true;
}

TestActionPtr TimeoutTestAction::createAction() const
{
    return TestActionPtr();
}
