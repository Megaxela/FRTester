//
// Created by megaxela on 28.11.16.
//

#ifndef FRTESTER_TESTENVIROMENT_H
#define FRTESTER_TESTENVIROMENT_H


#include "TestDriver.h"

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
    TestEnvironment(TestDriver* driver);

    /**
     * @brief Метод для получения тестового драйвера.
     * @return
     */
    TestDriver* driver() const;

private:
    TestDriver* m_driver;
};


#endif //FRTESTER_TESTENVIROMENT_H
