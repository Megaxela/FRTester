//
// Created by xelam on 30.11.2016.
//

#include "include/Windows/Widgets/QTestsTreeWidget.h"
#include <QMouseEvent>
#include <QMenu>
#include <include/Tools/Logger.h>

QTestsTreeWidget::QTestsTreeWidget(QWidget *parent) :
        QTreeWidget(parent)

{
    m_testsMenu = new QTestsTreeWidgetMenu(this);

    connect(m_testsMenu,
            &QTestsTreeWidgetMenu::selectionEnabledChanged,
            this,
            &QTestsTreeWidget::OnContextEnableStateChanged);

    m_testsRootItem = new QTreeWidgetItem(this);
    m_testsRootItem->setText(0, "Тесты");
    m_testsRootItem->setFlags(
            Qt::ItemIsEnabled
    );

    m_triggersRootItem = new QTreeWidgetItem(this);
    m_triggersRootItem->setText(0, "Триггеры");
    m_triggersRootItem->setFlags(
            Qt::ItemIsEnabled
    );

    clearTests();
}

QTestsTreeWidget::~QTestsTreeWidget()
{
    delete m_testsMenu;
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
}

void QTestsTreeWidget::addTrigger(TriggerTestPtr trigger)
{
    TriggerData data;
    data.trigger = trigger;
    data.item = new QTreeWidgetItem(m_triggersRootItem);
    data.item->setText(0, QString::fromStdString(trigger->name()));
    data.item->setWhatsThis(0, QString::fromStdString(trigger->description()));
    data.enabled = true;

    m_triggers.push_back(data);
}

void QTestsTreeWidget::addTest(TestPtr test)
{
    TestData data;
    data.test = test;
    data.item = new QTreeWidgetItem(m_testsRootItem);
    data.item->setText(0, QString::fromStdString(test->name()));
    data.item->setWhatsThis(0, QString::fromStdString(test->description()));
    data.enabled = true;

    m_tests.push_back(data);
}

void QTestsTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        auto items = selectedItems();

        QTreeWidgetItem* pressedItem = itemAt(event->pos());

        if (pressedItem != nullptr)
        {
            if (pressedItem->flags() & Qt::ItemIsSelectable)
            {
                if (!items.contains(pressedItem))
                {
                    items.push_back(pressedItem);
                }
            }
        }

        if (items.length() == 0)
        {
            return;
        }

        m_testsMenu->setMultipleMode(items.length() > 1);

        m_testsMenu->move(event->globalPos());
        m_testsMenu->show();
    }

    QTreeWidget::mousePressEvent(event);
}

void QTestsTreeWidget::OnContextEnableStateChanged(bool state)
{
    auto items = selectedItems();

    if (items.length() == 0)
        return;

    QFont font = items[0]->font(0);

    font.setStrikeOut(!state);

    for (auto item : items)
    {
        item->setFont(0, font);

        // Поиск объектов и установка их значений
        for (auto test : m_tests)
        {
            if (test.item == item)
            {
                test.enabled = state;
            }
        }

        for (auto trigger : m_triggers)
        {
            if (trigger.item == item)
            {
                trigger.enabled = state;
            }
        }
    }
}
