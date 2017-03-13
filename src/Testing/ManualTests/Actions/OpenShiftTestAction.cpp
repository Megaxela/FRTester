//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/OpenShiftTestAction.h"

REGISTER_ACTION(OpenShiftTestAction);

OpenShiftTestAction::OpenShiftTestAction() :
    AbstractTestAction("Открытие смены",
                       "Действие, открывающее смену.",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

OpenShiftTestAction::~OpenShiftTestAction()
{

}

bool OpenShiftTestAction::execute()
{
    environment()->driver()->openShift(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr OpenShiftTestAction::createAction() const
{
    return std::make_shared<OpenShiftTestAction>();
}
