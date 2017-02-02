//
// Created by megaxela on 09.12.16.
//

#ifndef FRTESTER_CONNECTTCPTABCONTROLLER_H
#define FRTESTER_CONNECTTCPTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>
#include <include/Implementation/TCPInterface.h>

/**
 * @brief Класс для усравления вкладкой соединения по
 * RNDIS (TCPIP)
 */
class ConnectTCPTabController : public AbstractTabController
{
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

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * соединения с ФР.
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

protected:
    void setupConnections() override;

    void configureWidgets() override;

private:
    std::shared_ptr<TCPInterface> m_tcpInterface;
};


#endif //FRTESTER_CONNECTTCPTABCONTROLLER_H
