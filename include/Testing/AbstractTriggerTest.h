//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_TRIGGERTEST_H
#define FRTESTER_TRIGGERTEST_H

#include <memory>

class AbstractTriggerTest;

typedef std::shared_ptr<AbstractTriggerTest> TriggerTestPtr;

/**
 * @brief Класс, оборачивающий триггер тест.
 */
class AbstractTriggerTest
{
public:
    /**
     * @brief Конструктор.
     */
    AbstractTriggerTest(const std::string& name,
                        const std::string& description,
                        bool critical = false);

    /**
     * @brief Деструктор.
     */
    ~AbstractTriggerTest();

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
     * @brief Метод для проверки наличия тега среди
     * тегов теста.
     * @return Результат проверки наличия тега в тегах теста.
     */
    virtual bool containsTag(const std::string &tag) = 0;

    /**
     * @brief Метод для получения названия теста.
     * @return Строка с названием теста.
     */
    std::string name() const;

    /**
     * @brief Метод для получения описания теста.
     * @return Строка с описанием теста.
     */
    std::string description() const;

    /**
     * @brief Метод для получения критичности триггера.
     * Если тригер критичен - то тесты будут остановлены
     * после того как этот тригер найдет ошибку. Если
     * тригер не критичен - то тесты не будут остановлены
     * но факт того, что была найдена ошибка будет записана.
     * @return Критичность триггера.
     */
    bool isCritical() const;

    /**
     * @brief Метод для установки названия теста.
     * @param name Новое название теста.
     */
    void setName(const std::string& name);

    /**
     * @brief Метод для установки описания теста.
     * @param description Новое описание теста.
     */
    void setDescription(const std::string& description);

private:
    std::string m_name;

    std::string m_description;

    bool m_critical;
};


#endif //FRTESTER_TRIGGERTEST_H
