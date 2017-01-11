//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_TESTDRIVER_H
#define FRTESTER_TESTDRIVER_H

#include <cstdint>
#include <Tools/ByteArray.h>
#include <vector>
#include <FRDriver.h>
#include "AbstractTriggerTest.h"

/**
 * @brief Класс, описывающий драйвер для тестирования.
 * Объект данного класса будет обрабатывать требуемые
 * триггеры при вызове функций.
 */
class TestDriver : public FRDriver
{
public:
    /**
     * @brief Конструктор.
     */
    TestDriver();

    /**
     * @brief Деструктор.
     */
    ~TestDriver();

    /**
     * @brief Метод для получения не тестового драйвера
     * без вызова тригеров.
     * @return Объект драйвера.
     */
    FRDriver* pureDriver();

protected:
    /**
     * @brief Метод для контроля вызова тестов - триггеров.
     * @param c Код вызываемой команды.
     * @param arguments Аргументы команды.
     * @return Результат вызова команды.
     */
    ByteArray sendCommand(const FRDriver::Command &c, const ByteArray &arguments, bool responseHasCashier) override;

    /**
     * @brief Метод для получения тега команды.
     * @param c
     * @return
     */
    std::string getCommandTag(const Command& c);

};


#endif //FRTESTER_TESTDRIVER_H
