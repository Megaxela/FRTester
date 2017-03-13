//
// Created by megaxela on 21.02.17.
//

#include "include/Testing/ManualTests/Actions/WhileTestAction.h"
#include <Testing/TestLogger.h>
#include <include/Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(WhileTestAction)

WhileTestAction::WhileTestAction() :
    AbstractTestAction("Цикл While",
                       "Действие, зациклено выполняет дочерние команды",
                       {{"Times", (uint32_t) 20}},
                       {CATEGORY_ADDITIONAL})
{
    setAllowChildren(true);
}

WhileTestAction::~WhileTestAction()
{

}

bool WhileTestAction::execute()
{
    auto childVector = children();

    for (uint32_t i = 0; i < getValue("Times").toUInt32(); ++i)
    {
        for (auto& action : childVector)
        {
            environment()->logger()->log("Выполняем \"" + action->getFullName() + "\"");

            // Выполняем действие
            bool result = action->execute();

            // Проверяем потребовало ли действие остановки
            // тестирования
            if (!result)
            {
                environment()->logger()->log("Действие \"" + action->getFullName() + "\" потребовало закончить тестирование.");
                return false;
            }

            // Проверяем, прервали ли тестирование
            if (environment()->tools()->testingStoped())
            {
                environment()->logger()->log("Ручной тест прерван.");
                return true;
            }
        }
    }

    return true;
}

TestActionPtr WhileTestAction::createAction() const
{
    return std::make_shared<WhileTestAction>();
}

std::string WhileTestAction::getFullName() const
{
    return name() + " с " + std::to_string(getValue("Times").toUInt32()) + " итерациями.";
}

void WhileTestAction::populateTreeWidgetItem(QTreeWidgetItem *item) const
{
    AbstractTestAction::populateTreeWidgetItem(item);
}
