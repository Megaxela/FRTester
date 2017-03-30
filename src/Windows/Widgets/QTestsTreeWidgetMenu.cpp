//
// Created by megaxela on 01.12.16.
//

#include "Windows/Widgets/QTestsTreeWidgetMenu.h"

QTestsTreeWidgetMenu::QTestsTreeWidgetMenu(QWidget *parent) :
        QMenu(parent),
        m_multipleMode(false)
{
    addAction("Включить", [=]()
    {
        emit selectionEnabledChanged(true);
    });

    addAction("Отключить", [=]()
    {
        emit selectionEnabledChanged(false);
    });

    addAction("Запустить", [=]()
    {
        emit selectionExecuted();
    });
}

void QTestsTreeWidgetMenu::setMultipleMode(bool state)
{
    m_multipleMode = state;
}

