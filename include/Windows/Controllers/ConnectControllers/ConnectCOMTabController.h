//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_CONNECTCOMTABCONTROLLER_H
#define FRTESTER_CONNECTCOMTABCONTROLLER_H


#include <Implementation/COMInterface.h>
#include "include/Windows/Controllers/AbstractTabController.h"

namespace Ui
{
    class MainWindow;
}

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

private slots:
    void onConnecting();

    void onDeviceEditingFinished();

    void onBaudRateEditingFinished();

protected:
    void configureWidgets() override;

    void setupConnections() override;

private:

    std::shared_ptr<COMInterface> m_comInterface;
};


#endif //FRTESTER_CONNECTCOMTABCONTROLLER_H
