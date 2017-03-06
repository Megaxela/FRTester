//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/PrintAdsTestAction.h"

REGISTER_ACTION(PrintAdsTestAction);

PrintAdsTestAction::PrintAdsTestAction() :
    AbstractTestAction("Метод печати рекламного теста",
                       "",
                       {{"Пароль", (uint32_t) 30}})
{

}

PrintAdsTestAction::~PrintAdsTestAction()
{

}

bool PrintAdsTestAction::execute()
{
    environment()->driver()->printAds(
            getValue("Пароль").toUInt32()
    );

    return true;
}

TestActionPtr PrintAdsTestAction::createAction() const
{
    return std::make_shared<PrintAdsTestAction>();
}
