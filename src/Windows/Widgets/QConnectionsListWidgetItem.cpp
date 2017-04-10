//
// Created by megaxela on 10.04.17.
//

#include "Windows/Widgets/QConnectionsListWidgetItem.h"

const int32_t QConnectionsListWidgetItem::type = 0xC0C0;

QConnectionsListWidgetItem::QConnectionsListWidgetItem(QListWidget *parent, std::shared_ptr<Connection> connection) :
    QListWidgetItem(parent, type),
    m_connection(connection)
{
    updateText();
}

QConnectionsListWidgetItem::~QConnectionsListWidgetItem()
{

}

void QConnectionsListWidgetItem::updateText()
{
    setText(QString::fromStdString(m_connection->name()));
}
