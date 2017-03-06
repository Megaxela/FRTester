//
// Created by megaxela on 27.02.17.
//

#include "include/Testing/ManualTests/Actions/OpenShiftTestAction.h"

OpenShiftTestAction::OpenShiftTestAction() :
    AbstractTestAction("Открытие смены",
                       "Действие, открывающее смену.",
                       {{"Пароль", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

OpenShiftTestAction::~OpenShiftTestAction()
{

}

bool OpenShiftTestAction::execute()
{
    environment()->driver()->openShift(
            getValue("Пароль").toUInt32()
    );

    return true;
}

TestActionPtr OpenShiftTestAction::createAction() const
{
    return TestActionPtr();
}
