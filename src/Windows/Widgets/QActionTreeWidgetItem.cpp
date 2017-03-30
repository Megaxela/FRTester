//
// Created by megaxela on 20.02.17.
//

#include "Windows/Widgets/QActionTreeWidgetItem.h"

QActionTreeWidgetItem::QActionTreeWidgetItem(QTreeWidget *view, TestActionPtr action) :
        QTreeWidgetItem(view, ActionType),
        m_action(action)
{

}

QActionTreeWidgetItem::QActionTreeWidgetItem(QTreeWidgetItem *item, TestActionPtr action) :
        QTreeWidgetItem(item, ActionType),
        m_action(action)
{

}

QActionTreeWidgetItem::~QActionTreeWidgetItem()
{

}

TestActionPtr QActionTreeWidgetItem::getAction() const
{
    return m_action;
}

