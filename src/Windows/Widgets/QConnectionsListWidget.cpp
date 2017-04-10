//
// Created by megaxela on 10.04.17.
//

#include <Windows/Widgets/QConnectionsListWidgetItem.h>
#include <Testing/ConnectionsManager/ConnectionsManager.h>
#include "Windows/Widgets/QConnectionsListWidget.h"

QConnectionsListWidget::QConnectionsListWidget(QWidget *parent) :
        QListWidget(parent)
{
    // Начальное заполнение
    for (uint32_t i = 0;
         i < ConnectionsManager::instance().numberOfConnections();
         ++i)
    {
        addItem(new QConnectionsListWidgetItem(
                this,
                ConnectionsManager::instance().getConnection(i)
        ));
    }
}

QConnectionsListWidget::~QConnectionsListWidget()
{

}

void QConnectionsListWidget::updateNames()
{
    for (int32_t i = 0; i < count(); ++i)
    {
        auto itemElement = item(i);

        if (itemElement->type() == QConnectionsListWidgetItem::type)
        {
            static_cast<QConnectionsListWidgetItem*>(itemElement)->updateText();
        }
    }
}
