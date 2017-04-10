//
// Created by megaxela on 10.04.17.
//

#ifndef FRTESTER_QCONNECTIONSLISTWIDGET_H
#define FRTESTER_QCONNECTIONSLISTWIDGET_H

#include <QListWidget>

/**
 * @brief Виджет, предоставляющий изображение
 * текущих доступных соединений.
 */
class QConnectionsListWidget : public QListWidget
{
public:
    /**
     * @brief Конструктор.
     * @param parent Родительский виджет.
     */
    QConnectionsListWidget(QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~QConnectionsListWidget();

    /**
     * @brief Метод для обновления имен соединений.
     */
    void updateNames();
};


#endif //FRTESTER_QCONNECTIONSLISTWIDGET_H
