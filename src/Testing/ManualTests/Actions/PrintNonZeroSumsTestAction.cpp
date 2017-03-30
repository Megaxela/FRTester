//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/PrintNonZeroSumsTestAction.h"

REGISTER_ACTION(PrintNonZeroSumsTestAction)

PrintNonZeroSumsTestAction::PrintNonZeroSumsTestAction() :
    AbstractTestAction("Вывод ненулевых сумм",
                        "",
                       {},
                       {CATEGORY_ACTIONS})
{

}

PrintNonZeroSumsTestAction::~PrintNonZeroSumsTestAction()
{

}

bool PrintNonZeroSumsTestAction::execute()
{
    auto result = environment()->driver()->getNonZeroSums();

    environment()->logger()->log(
            "Продажи: " + std::to_string(result.incomingSum)
    );
    environment()->logger()->log(
            "Покупки: " + std::to_string(result.consumptionSum)
    );
    environment()->logger()->log(
            "Возвраты продаж: " + std::to_string(result.returnIncomingSum)
    );
    environment()->logger()->log(
            "Возвраты покупок: " + std::to_string(result.returnConsumptionSum)
    );

    return true;
}

TestActionPtr PrintNonZeroSumsTestAction::createAction() const
{
    return std::make_shared<PrintNonZeroSumsTestAction>();
}
