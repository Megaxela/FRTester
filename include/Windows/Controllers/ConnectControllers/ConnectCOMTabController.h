//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_CONNECTCOMTABCONTROLLER_H
#define FRTESTER_CONNECTCOMTABCONTROLLER_H


#include <Implementation/COMInterface.h>
#include <Testing/ConnectionsManager/Connection.h>
#include "Windows/Controllers/AbstractTabController.h"

namespace Ui
{
    class MainWindow;
}

class ConnectTabController;

/**
 * @brief Класс для упправления вкладкой соединения.
 */
class ConnectCOMTabController : public AbstractTabController
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор
     * @param ptr Указатель на интерфейс главного окна.
     */
    explicit ConnectCOMTabController(Ui::MainWindow* ptr,
                                     QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~ConnectCOMTabController();

signals:

    void connectionAdded(std::shared_ptr<Connection> connection);

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * соединения с ККТ.
     */
    void onConnecting();

    /**
     * @brief Слот, вызываемый при окончании изменения пути к
     * устройству.
     */
    void onDeviceEditingFinished();

    /**
     * @brief Слот, вызываемый при окончании изменения ComboBox
     * со скоростью обмена.
     */
    void onBaudRateEditingFinished();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку добавления
     * соединения.
     */
    void onConnectionAdd();

protected:
    void configureWidgets() override;

    void setupConnections() override;


private:

    ConnectTabController* connectTabController();

    std::shared_ptr<COMInterface> m_comInterface;
};


#endif //FRTESTER_CONNECTCOMTABCONTROLLER_H
