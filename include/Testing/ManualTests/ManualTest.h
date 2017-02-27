//
// Created by megaxela on 17.02.17.
//

#ifndef FRTESTER_MANUALTEST_H
#define FRTESTER_MANUALTEST_H

#include <memory>
#include <include/Testing/AbstractTest.h>
#include "AbstractTestAction.h"

class ManualTest;

typedef std::shared_ptr<ManualTest> ManualTestPtr;

/**
 * @brief Класс, описывающий мануальный тест.
 */
class ManualTest : public AbstractTest
{
public:

    ManualTest(const std::string &name,
               const std::string &description);

    /**
     * @brief Метод для добавления действия.
     * @param testAction Действие.
     */
    void addTestAction(TestActionPtr testAction);

    /**
     * @brief Метод для удаления действия из теста.
     * @param index Индекс действия.
     */
    void removeTestAction(uint32_t index);

    /**
     * @brief Метод для вставки действия в тест.
     * @param index Индекс действия.
     */
    void insertTestAction(uint32_t index, TestActionPtr testAction);

    /**
     * @brief Метод для смены тестов местами.
     * @param aIndex Индекс первого теста.
     * @param bIndex Индекс второго теста.
     */
    void swapActions(uint32_t aIndex, uint32_t bIndex);

    /**
     * @brief Метод для очистки всех действий.
     */
    void clear();

    /**
     * @brief Метод для получения действия из теста по индексу.
     * Если индекс выходит за пределы (index > numberOfActions) -
     * то будет выброшено исключение std::out_of_range.
     * @param index Индекс теста.
     * @return Действие.
     */
    TestActionPtr getTestAction(uint32_t index);

    /**
     * @brief Метод для получения количества действий,
     * используемых в тесте.
     * @return Количество действий.
     */
    uint32_t numberOfActions() const;

    bool execute() override;

private:
    void applyTest(TestActionPtr action, ManualTest* test);

    std::vector<TestActionPtr> m_testActions;
};


#endif //FRTESTER_MANUALTEST_H
