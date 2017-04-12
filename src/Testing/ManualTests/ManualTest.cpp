//
// Created by megaxela on 17.02.17.
//

#include "Testing/ManualTests/ManualTest.h"
#include <Testing/TestLogger.h>

ManualTest::ManualTest(const std::string &name,
                       const std::string &description) :
        AbstractTest(name, description, {MANUAL_TESTS_GROUP}),
        m_testActions()
{}

void ManualTest::addTestAction(TestActionPtr testAction)
{
    applyTest(testAction, this);
    m_testActions.push_back(testAction);
}

void ManualTest::removeTestAction(uint32_t index)
{
    applyTest(m_testActions[index], nullptr);
    m_testActions.erase(m_testActions.begin() + index);
}

void ManualTest::insertTestAction(uint32_t index, TestActionPtr testAction)
{
    if (index > numberOfActions())
    {
        throw std::out_of_range(std::to_string(index) + " > " + std::to_string(numberOfActions()));
    }

    applyTest(testAction, this);
    m_testActions.insert(m_testActions.begin() + index, testAction);
}

TestActionPtr ManualTest::getTestAction(uint32_t index)
{
    if (index >= numberOfActions())
    {
        throw std::out_of_range(std::to_string(index) + " >= " + std::to_string(numberOfActions()));
    }

    return m_testActions[index];
}

uint32_t ManualTest::numberOfActions() const
{
    return (uint32_t) m_testActions.size();
}

void ManualTest::swapActions(uint32_t aIndex, uint32_t bIndex)
{
    if (aIndex >= numberOfActions())
    {
        throw std::out_of_range(std::to_string(aIndex) + " >= " + std::to_string(numberOfActions()));
    }

    if (bIndex >= numberOfActions())
    {
        throw std::out_of_range(std::to_string(bIndex) + " >= " + std::to_string(numberOfActions()));
    }

    auto temp = m_testActions[aIndex];
    m_testActions[aIndex] = m_testActions[bIndex];
    m_testActions[bIndex] = temp;
}

bool ManualTest::execute()
{
    for (auto& action : m_testActions)
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

    return true;
}

void ManualTest::clear()
{
    m_testActions.clear();
}

void ManualTest::applyTest(TestActionPtr action, ManualTest* test)
{
    action->setManualTest(test);
    auto children = action->children();
    for (auto &child : children)
    {
        child->setManualTest(test);
    }
}

ManualTest::~ManualTest()
{

}
