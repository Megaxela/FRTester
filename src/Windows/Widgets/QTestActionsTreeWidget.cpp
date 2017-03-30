//
// Created by megaxela on 17.02.17.
//

#include <QtWidgets/QTreeWidget>
#include <QWidget>
#include <QTreeView>
#include <QMimeData>
#include <Testing/ManualTests/Actions/BeepTestAction.h>
#include <Tools/Logger.h>
#include <Windows/Widgets/QActionTreeWidgetItem.h>
#include <Testing/ManualTests/Actions/LongStateRequestTestAction.h>
#include <Testing/ManualTests/Actions/WhileTestAction.h>
#include <Testing/ManualTests/TestActionFabric.h>
#include "Windows/Widgets/QTestActionsTreeWidget.h"

QTestActionsTreeWidget::QTestActionsTreeWidget(QWidget *parent) :
        QTreeWidget(parent),
        m_searchFilter(),
        m_action()
{

    auto result = TestActionFabric::instance().getNames();

    for (auto& actionName : result)
    {
        auto action = TestActionFabric::instance().create(actionName);

        QVector<QString> category;

        for (auto& categoryName : action->category())
        {
            category.push_back(QString::fromStdString(categoryName));
        }

        m_action.push_back(Action(
                category,
                action
        ));
    }

    displayActions();

    connect(this,
            &QTestActionsTreeWidget::currentItemChanged,
            this,
            &QTestActionsTreeWidget::onElementSelected);
}

QTestActionsTreeWidget::~QTestActionsTreeWidget()
{

}

void QTestActionsTreeWidget::setSearchFilter(const QString &string)
{
    if (string.isEmpty())
    {
        m_searchFilter = QRegExp();
    }
    else
    {
        m_searchFilter = QRegExp(string);
    }
    displayActions();
}

void QTestActionsTreeWidget::displayActions()
{
    clear();
    for (auto iterator = m_action.begin(), end = m_action.end();
         iterator != end;
         ++iterator)
    {
        auto &element = *iterator;

        element.item = nullptr;
        if (m_searchFilter.indexIn(QString::fromStdString(element.action->name())) == -1)
        {
            continue;
        }

        // Проверяем наличие topLevel элемента (если требуется)
        QTreeWidgetItem* topElement = nullptr;
        if (element.categories.size() != 0)
        {
            bool found = false;
            for (int i = 0; i < topLevelItemCount() && !found; ++i)
            {
                auto topLevelItemPtr = topLevelItem(i);
                if (element.categories[0] == topLevelItemPtr->text(0))
                {
                    topElement = topLevelItemPtr;
                    found = true;
                }
            }

            if (!found)
            {
                topElement = new QTreeWidgetItem(this);
                topElement->setText(0, element.categories[0]);
                addTopLevelItem(topElement);
            }
        }
        else
        {
            topElement = new QActionTreeWidgetItem(this, element.action);
            topElement->setText(0, QString::fromStdString(element.action->name()));
            element.item = topElement;
            addTopLevelItem(topElement);
        }

        // Ищем/формируем дочерние объекты (если требуется)
        for (int i = 1; i < element.categories.size(); ++i)
        {
            bool found = false;
            for (int childIndex = 0;
                 childIndex < topElement->childCount() && !found;
                 ++childIndex)
            {
                if (topElement->child(childIndex)->text(0) != element.categories[i])
                {
                    continue;
                }

                topElement = topElement->child(childIndex);
                found = true;
            }

            if (!found)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(topElement);
                item->setText(0, element.categories[i]);
                topElement = item;
            }
        }

        if (element.item == nullptr)
        {
            QTreeWidgetItem* actionItem = new QActionTreeWidgetItem(topElement, element.action);
            actionItem->setText(0, QString::fromStdString(element.action->name()));
            element.item = actionItem;
        }
    }
}

QMimeData *QTestActionsTreeWidget::mimeData(const QList<QTreeWidgetItem *> items) const
{
    int type = items[0]->type() == QActionTreeWidgetItem::Type; // 0 - action, 1 - category, 2 - mixed

    for (auto& element : items)
    {
        if (((type == 0) && (element->type() == QActionTreeWidgetItem::Type)) ||
            ((type == 1) && (element->type() == QActionTreeWidgetItem::ActionType)))
        {
            type = 2;
            break;
        }
    }

    auto mime = new QMimeData();

    switch (type)
    {
    case 0:
        mime->setText("testing/action");
        break;

    case 1:
        mime->setText("testing/category");
        break;

    case 2:
        mime->setText("testing/mixed");
        break;
    default:
        Error("Unknown type for mime.");
        return mime;
    }

    return mime;
}

void QTestActionsTreeWidget::onElementSelected(QTreeWidgetItem *item)
{
    if (item && item->type() == QActionTreeWidgetItem::ActionType)
    {
        onActionSelected(
                static_cast<QActionTreeWidgetItem*>(
                        item
                )->getAction()
        );
    }
    else
    {
        onActionSelected(nullptr);
    }
}
