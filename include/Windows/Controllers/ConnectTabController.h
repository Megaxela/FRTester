//
// Created by megaxela on 08.12.16.
//

#ifndef FRTESTER_CONNECTTABCONTROLLER_H
#define FRTESTER_CONNECTTABCONTROLLER_H

#include <QWidget>
#include <QTabWidget>
#include <Testing/ConnectionsManager/Connection.h>
#include "AbstractTabController.h"

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс для контроллирования вкладки с соединением.
 */
class ConnectTabController : public AbstractTabController
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на интерфейс.
     * @param parent Родительский виджет.
     * @param tabWidget Подконтрольный виджет.
     */
    ConnectTabController(Ui::MainWindow* ptr,
                         QWidget* parent,
                         QTabWidget* tabWidget);

    /**
     * @brief Деструктор.
     */
    ~ConnectTabController();

    /**
     * @brief Метод для получения информации об
     * устройстве и вывода его на экран.
     * @return Успешность получения информации.
     */
    bool receiveDeviceInfo();

    void tabSelected() override;

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    void onConnectionAdded(std::shared_ptr<Connection> connection);

    void onFlashCompletePushButtonPressed();

    void onZeroingPushButtonPressed();

    void onSetTimePushButtonPressed();
};


#endif //FRTESTER_CONNECTTABCONTROLLER_H
