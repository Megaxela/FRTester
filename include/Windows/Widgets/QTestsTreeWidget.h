//
// Created by xelam on 30.11.2016.
//

#ifndef FRTESTER_TESTSTREEWIDGET_H
#define FRTESTER_TESTSTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <include/Testing/AbstractTest.h>
#include <include/Testing/AbstractTriggerTest.h>

/**
 * @brief Класс, описывающий древовидное меню
 * выбора тестов.
 */
class QTestsTreeWidget : public QTreeWidget
{
public:
    /**
     * @brief Конструктор
     * @param parent
     */
    QTestsTreeWidget(QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~QTestsTreeWidget();

    /**
     * @brief Метод для очистки виджета от данных.
     */
    void clearTests();

    /**
     * @brief Метод для добавления триггера в виджет.
     * @param trigger Триггер.
     */
    void addTrigger(TriggerTestPtr trigger);

    /**
     * @brief Метод для добавления теста в виджет.
     * @param test
     */
    void addTest(TestPtr test);

protected:


private:
    struct TestData
    {
        TestPtr test;
        QTreeWidgetItem* item;
    };

    struct TriggerData
    {
        TriggerTestPtr trigger;
        QTreeWidgetItem* item;
    };

    QVector<TestData> m_tests;
    QVector<TriggerData> m_triggers;

    QTreeWidgetItem* m_testsRootItem;
    QTreeWidgetItem* m_triggersRootItem;
};


#endif //FRTESTER_TESTSTREEWIDGET_H
