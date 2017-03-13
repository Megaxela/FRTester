//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/PrintAdsTestAction.h"

REGISTER_ACTION(PrintAdsTestAction)

PrintAdsTestAction::PrintAdsTestAction() :
    AbstractTestAction("Печать рекламного теста",
                       "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

PrintAdsTestAction::~PrintAdsTestAction()
{

}

bool PrintAdsTestAction::execute()
{
    environment()->driver()->printAds(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr PrintAdsTestAction::createAction() const
{
    return std::make_shared<PrintAdsTestAction>();
}
