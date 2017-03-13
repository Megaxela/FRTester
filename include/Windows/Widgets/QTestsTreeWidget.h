//
// Created by xelam on 30.11.2016.
//

#ifndef FRTESTER_TESTSTREEWIDGET_H
#define FRTESTER_TESTSTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <include/Testing/AbstractTest.h>
#include <include/Testing/AbstractTriggerTest.h>
#include "QTestsTreeWidgetMenu.h"

Q_DECLARE_METATYPE(TestPtr)
Q_DECLARE_METATYPE(TriggerTestPtr)

/**
 * @brief Класс, описывающий древовидное меню
 * выбора тестов.
 */
class QTestsTreeWidget : public QTreeWidget
{
    Q_OBJECT
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
signals:
    void selectedTestsExecuted(QVector<TestPtr> tests);

    void testSelected(TestPtr test);

    void triggerSelected(TriggerTestPtr test);

public slots:
    void onContextEnableStateChanged(bool state);

    void onCurrentTestExecuted();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QTestsTreeWidget(const QTestsTreeWidget&) = delete;
    QTestsTreeWidget& operator=(const QTestsTreeWidget&) = delete;

    struct TestData
    {
        TestData() :
            test(),
            item(nullptr),
            enabled(true)
        {}

        TestData(const TestData& rhs) :
            test(rhs.test),
            item(rhs.item),
            enabled(rhs.enabled)
        {}

        TestData& operator=(const TestData& rhs)
        {
            test = rhs.test;
            item = rhs.item;
            enabled = rhs.enabled;

            return (*this);
        }

        TestPtr test;
        QTreeWidgetItem* item;
        bool enabled;
    };

    struct TriggerData
    {
        TriggerData() :
            trigger(),
            item(nullptr),
            enabled(true)
        {}

        TriggerData(const TriggerData& rhs) :
            trigger(rhs.trigger),
            item(rhs.item),
            enabled(rhs.enabled)
        {}

        TriggerData& operator=(const TriggerData& rhs)
        {
            trigger = rhs.trigger;
            item = rhs.item;
            enabled = rhs.enabled;

            return (*this);
        }

        TriggerTestPtr trigger;
        QTreeWidgetItem* item;
        bool enabled;
    };

    QVector<TestData> m_tests;
    QVector<TriggerData> m_triggers;

    QTreeWidgetItem* m_testsRootItem;
    QTreeWidgetItem* m_triggersRootItem;

    QTestsTreeWidgetMenu* m_testsMenu;
};


#endif //FRTESTER_TESTSTREEWIDGET_H
