//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_CYCLETEST_H
#define FRTESTER_CYCLETEST_H


#include <Testing/AbstractTest.h>
#include <Testing/TestEnvironment.h>

/**
 * @brief Тест, обрабатывающий следующий кейс:
 * При закрытии чека с одной продажей стоимостью 16 и оплате
 * суммами 4/4/4/4 то ФР уходит в цикл.
 */
class CycleTest : public AbstractTest
{
public:
    CycleTest();

    /**
     * @brief Конструктор.
     * @param driver Указатель на тестировочное окружение.
     */
    CycleTest(TestEnvironment* environment);

    /**
     * @brief Метод выполняющий тест.
     * @return
     */
    bool execute() override;
};


#endif //FRTESTER_CYCLETEST_H
