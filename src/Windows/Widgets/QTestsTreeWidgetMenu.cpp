//
// Created by megaxela on 01.12.16.
//

#include "include/Windows/Widgets/QTestsTreeWidgetMenu.h"

QTestsTreeWidgetMenu::QTestsTreeWidgetMenu(QWidget *parent) :
        QMenu(parent)
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

