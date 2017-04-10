//
// Created by megaxela on 10.04.17.
//

#ifndef FRTESTER_QCONNECTIONSLISTWIDGETITEM_H
#define FRTESTER_QCONNECTIONSLISTWIDGETITEM_H

#include <QListWidgetItem>
#include <Testing/ConnectionsManager/Connection.h>

class QConnectionsListWidgetItem : public QListWidgetItem
{
public:

    static const int32_t type;

    /**
     * @brief Конструктор.
     * @param parent Родительский объект.
     * @param connection Умный указатель на соединение.
     */
    QConnectionsListWidgetItem(QListWidget* parent, std::shared_ptr<Connection> connection);

    /**
     * @brief Деструктор.
     */
    ~QConnectionsListWidgetItem();

    /**
     * @brief Метод для обновления текста. Если текст соединения изменился.
     */
    void updateText();

private:
    std::shared_ptr<Connection> m_connection;
};


#endif //FRTESTER_QCONNECTIONSLISTWIDGETITEM_H
