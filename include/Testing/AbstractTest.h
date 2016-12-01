//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_ABSTRACTTEST_H
#define FRTESTER_ABSTRACTTEST_H

#include <memory>
#include "TestDriver.h"
#include "TestEnvironment.h"

class AbstractTest;

typedef std::shared_ptr<AbstractTest> TestPtr;

/**
 * @brief Класс, описывающий тест.
 */
class AbstractTest
{
public:
    /**
     * @brief Конструктор.
     * @param driver Драйвер для вызова функций.
     */
    AbstractTest(TestEnvironment *environment,
                 const std::string &name,
                 const std::string &description);

    /**
     * @brief Деструктор.
     */
    ~AbstractTest();

    /**
     * @brief Виртуальный метод, выполняющий тест.
     * Устройство находится в начальном режиме. Оно
     * всегда будет находиться в начальном режиме.
     * Если не удасться перевести измененное состояние
     * устройства в начальное то следующий тест просто
     * не будет запущен.
     * @return Результат тестирования.
     */
    virtual bool execute() = 0;

    /**
     * @brief Метод для получения названия теста.
     * @return Название теста.
     */
    std::string name() const;

    /**
     * @brief Метод для получения описания теста.
     * @return Описание теста.
     */
    std::string description() const;

    /**
     * @brief Метод для установки названия теста.
     * @param name Название теста.
     */
    void setName(const std::string& name);

    /**
     * @brief Метод для установки описания теста.
     * @param description Описание теста.
     */
    void setDescription(const std::string& description);

protected:
    /**
     * @brief Метод для получения драйвера.
     * @return
     */
    TestEnvironment* enviroment();

private:
    TestEnvironment* m_enviroment;

    std::string m_name;

    std::string m_description;
};


#endif //FRTESTER_ABSTRACTTEST_H
