//
// Created by megaxela on 28.11.16.
//

#ifndef FRTESTER_TESTENVIROMENT_H
#define FRTESTER_TESTENVIROMENT_H


#include "TestDriver.h"

class TestLogger;
/**
 * @brief Класс, описывающий тестовое окружение.
 * Весь инструментарий, доступный для тестирования
 * будет храниться здесь.
 */
class TestEnvironment
{
public:
    /**
     * @brief Конструктор.
     * @param driver Тестовый драйвер.
     */
    TestEnvironment(
            TestDriver* driver,
            TestLogger* logger
    );

    /**
     * @brief Метод для получения тестового драйвера.
     * @return
     */
    TestDriver* driver() const;

    /**
     * @brief
     * @return
     */
    TestLogger* logger() const;

private:
    TestDriver* m_driver;
    TestLogger* m_logger;
};


#endif //FRTESTER_TESTENVIROMENT_H
