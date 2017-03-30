//
// Created by megaxela on 17.02.17.
//

#ifndef FRTESTER_ABSTRACTTESTACTION_H
#define FRTESTER_ABSTRACTTESTACTION_H

#include <Testing/TestDataValue.h>
#include <string>
#include <vector>
#include <Testing/TestEnvironment.h>



#define CATEGORY_ACTIONS "Действия"
#define CATEGORY_COMMANDS "Команды"
#define CATEGORY_ADDITIONAL "Дополнительно"

class ManualTest;
class AbstractTestAction;
class QTreeWidgetItem;

typedef std::shared_ptr<AbstractTestAction> TestActionPtr;

/**
 * @brief Класс, описывающий абстрактное действие
 * в тесте.
 */
class AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    AbstractTestAction(
            const std::string &name,
            const std::string &description,
            const std::vector<std::pair<std::string, DataValue>> &fields,
            const std::vector<std::string> category
    );

    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~AbstractTestAction();

    /**
     * @brief Метод для получения названия действия.
     * @return Название действия.
     */
    std::string name() const;

    /**
     * @brief Виртуальный метод для формирования полного
     * названия действия. По умолчанию - возвращает просто
     * название действия. Пример использования:
     * Название: Таймаут
     * Полное название: Таймаут на 200мс
     * @return Полное название.
     */
    virtual std::string getFullName() const;

    /**
     * @brief Метод для получения описания действия.
     * @return Описание действия.
     */
    std::string description() const;

    /**
     * @brief Метод для получения переменных действия.
     * @return Массив из пар - (название, тип)
     */
    std::vector<std::pair<std::string, DataValue::Type>> getVariables() const;

    /**
     * @brief Метод для получения всех переменных и их значений.
     * @return
     */
    std::vector<std::pair<std::string, DataValue>> getAllVariables() const;

    /**
     * @brief Метод для устаноки значения переменной. Если типы
     * переменной и значения не будут совпадать - то будет
     * выброшено исключение std::invalid_argument.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, const DataValue& value);

    /**
     * @brief Метод для получения значения переменной. Если переменная
     * не найдено, будет выброшено исключение std::invalid_argument.
     * @param name Название переменной.
     * @return Значение переменной.
     */
    DataValue getValue(const std::string& name) const;

    /**
     * @brief Метод для получение тестировочного окружения.
     * @return Указатель на объект тестировочного окружения.
     */
    TestEnvironment* environment() const;

    /**
     * @brief Метод для передачи тестировочного окружения.
     * @param environment Указатель на тестировочного окружение.
     */
    void setEnvironment(TestEnvironment* environment);

    /**
     * @brief Метод вызываемый при исполнении действия.
     * @return Продолжать ли тестирование после выполнения
     */
    virtual bool execute() = 0;

    /**
     * @brief Метод вызываемый при добавлении элемента в
     * тест. Требуется для кастомизации отображения элемента.
     * По умолчанию - это простой TreeWidgetItem с fullName текстом.
     */
    virtual void populateTreeWidgetItem(QTreeWidgetItem* item) const;

    /**
     * @brief Метод для получения дочерних действий.
     * @return Массив с дочерними действиями.
     */
    std::vector<TestActionPtr> children() const;

    /**
     * @brief Метод для установки теста, к которому
     * принадлежит это действие.
     * @param test Тест.
     */
    void setManualTest(ManualTest* test);

    /**
     * @brief Метод для копирования действия.
     * @return Независимая копия действия.
     */
    TestActionPtr copy() const;

    /**
     * @brief Метод для добавления дочернего действия.
     * @param action Действие.
     */
    void addChild(TestActionPtr action);

    /**
     * @brief Метод для вставки дочернего действия.
     * @param action Действие.
     * @param index Место для вставки.
     */
    void insertChild(TestActionPtr action, uint32_t index);

    /**
     * @brief Метод для удаления дочернего объекта.
     * @param index Индекс удаляемого дочернего объекта.
     */
    void removeChild(uint32_t index);

    /**
     * @brief Метод для очистки дочерних объектов.
     */
    void clearChildren();

    /**
     * @brief Метод для проверки, требуются ли для
     * этого действия дочерние объекты.
     * @return
     */
    bool allowChildren() const;

    /**
     * @brief Метод для получения категории действия.
     * @return Массив с категориями
     */
    std::vector<std::string> category() const;

protected:

    virtual TestActionPtr createAction() const = 0;

    void setAllowChildren(bool allow);

private:
    AbstractTestAction(const AbstractTestAction&) = delete;
    AbstractTestAction& operator=(const AbstractTestAction&) = delete;


    void applyTest(TestActionPtr action, ManualTest* test);

    ManualTest* m_manualTest;

    bool m_allowChildren;
    std::string m_name;
    std::string m_description;
    std::vector<std::pair<std::string, DataValue>> m_dynamicValues;
    std::vector<TestActionPtr> m_childrenActions;
    std::vector<std::string> m_category;
};


#endif //FRTESTER_ABSTRACTTESTACTION_H
