//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_TRIGGERTEST_H
#define FRTESTER_TRIGGERTEST_H

#include <memory>

class TriggerTest;

typedef std::shared_ptr<TriggerTest> TriggerTestPtr;

/**
 * @brief Класс, оборачивающий Python триггер тест.
 */
class TriggerTest
{
public:

    /**
     * @brief Деструктор.
     */
    ~TriggerTest();

    /**
     * @brief Метод, вызываемый перед выполнением
     * команды на который данный триггер повешен.
     */
    virtual void onPreExecute() = 0;

    /**
     * @brief Метод, вызываемый после выполнения
     * команды на который данный триггер повешен.
     */
    virtual void onPostExecute() = 0;

    /**
     * @brief Метод для получения результата тестирования.
     * @return true при успешном тесте и false при неуспешном
     */
    virtual bool succeed() = 0;

    /**
     * @brief Метод для получения тега теста. По которому
     * будет производиться выборка тестов.
     * @return Тег теста.
     */
    virtual bool containsTag(const std::string &tag);

private:

    /**
     * @brief Конструктор.
     */
    TriggerTest();


};


#endif //FRTESTER_TRIGGERTEST_H
