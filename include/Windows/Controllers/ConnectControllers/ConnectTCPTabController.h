//
// Created by megaxela on 09.12.16.
//

#ifndef FRTESTER_CONNECTTCPTABCONTROLLER_H
#define FRTESTER_CONNECTTCPTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <Implementation/TCPInterface.h>
#include <Testing/ConnectionsManager/Connection.h>

class ConnectTabController;

/**
 * @brief Класс для усравления вкладкой соединения по
 * RNDIS (TCPIP)
 */
class ConnectTCPTabController : public AbstractTabController
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на интерфейс.
     * @param parent Родительский виджет.
     * @param tabWidget Подконтрольный виджет
     */
    ConnectTCPTabController(Ui::MainWindow *ptr,
                            QWidget *parent,
                            QTabWidget *tabWidget=nullptr);

    /**
     * @brief Деструктор.
     */
    ~ConnectTCPTabController();

signals:

    void connectionAdded(std::shared_ptr<Connection> connection);

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * соединения с ККТ.
     */
    void onConnecting();

    /**
     * @brief Слот, вызываемый при окончании редактирования
     * поля с IP адресом для его сохранения.
     */
    void onIPEditingFinished();

    /**
     * @brief Слот, вызываемый при окончании редактирования
     * поля с портом для его сохранения.
     */
    void onPortEditingFinished();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку доба
     */
    void onConnectionAdd();

protected:
    void setupConnections() override;

    void configureWidgets() override;

private:
    ConnectTabController* connectTabController();

    std::shared_ptr<TCPInterface> m_tcpInterface;
};


#endif //FRTESTER_CONNECTTCPTABCONTROLLER_H
