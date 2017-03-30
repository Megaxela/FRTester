//
// Created by megaxela on 22.02.17.
//

#include <Tools/Time.h>
#include <Testing/ManualTests/TestActionFabric.h>
#include <Testing/ManualTests/Actions/TimeoutTestAction.h>

REGISTER_ACTION(TimeoutTestAction)

TimeoutTestAction::TimeoutTestAction() :
    AbstractTestAction("Таймаут",
                       "Таймаут в мс.",
                       {{"Timeout (ms)", (uint32_t) 1000}},
                       {CATEGORY_ADDITIONAL})
{

}

TimeoutTestAction::~TimeoutTestAction()
{

}

bool TimeoutTestAction::execute()
{
    Time::sleep<std::chrono::milliseconds>(
            getValue("Timeout (ms)").toUInt32()
    );

    return true;
}

TestActionPtr TimeoutTestAction::createAction() const
{
    return std::make_shared<TimeoutTestAction>();
}
