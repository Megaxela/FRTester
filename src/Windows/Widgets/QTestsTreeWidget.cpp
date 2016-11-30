//
// Created by xelam on 30.11.2016.
//

#include "include/Windows/Widgets/QTestsTreeWidget.h"

QTestsTreeWidget::QTestsTreeWidget(QWidget *parent) :
        QTreeWidget(parent)

{
    m_testsRootItem = new QTreeWidgetItem(this);
    m_testsRootItem->setText(0, "Тесты");

    m_triggersRootItem = new QTreeWidgetItem(this);
    m_triggersRootItem->setText(0, "Триггеры");

    clearTests();
}

QTestsTreeWidget::~QTestsTreeWidget()
{

}

void QTestsTreeWidget::clearTests()
{
    for (auto trigger : m_triggers)
    {
        m_triggersRootItem->removeChild(trigger.item);
        delete trigger.item;
    }

    for (auto tests : m_tests)
    {
        m_testsRootItem->removeChild(tests.item);
        delete tests.item;
    }

    m_triggers.clear();
    m_tests.clear();
    clear();
}

void QTestsTreeWidget::addTrigger(TriggerTestPtr trigger)
{
    TriggerData data;
    data.trigger = trigger;
    data.item = new QTreeWidgetItem(this);
    data.item->setText(0, QString::fromStdString(trigger->name()));

    m_testsRootItem->addChild(data.item);

    m_triggers.push_back(data);
}

void QTestsTreeWidget::addTest(TestPtr test)
{
    TestData data;
    data.test = test;
    data.item = new QTreeWidgetItem(this);
    data.item->setText(0, QString::fromStdString(test->name()));

    m_tests.push_back(data);
}
