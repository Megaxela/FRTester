//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_CONNECTTABCONTROLLER_H
#define FRTESTER_CONNECTTABCONTROLLER_H


#include <Implementation/COMInterface.h>
#include "AbstractTabController.h"

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс для упправления вкладкой соединения.
 */
class ConnectTabController : public AbstractTabController
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор
     * @param ptr Указатель на интерфейс главного окна.
     */
    explicit ConnectTabController(Ui::MainWindow* ptr, QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~ConnectTabController();

private slots:
    void onConnecting();

private:
    void setupConnections() override;

protected:
    void configureWidgets() override;

private:

    void connectCOM();

    std::shared_ptr<COMInterface> m_comInterface;

    Ui::MainWindow* m_ui;
    QWidget* m_parent;
};


#endif //FRTESTER_CONNECTTABCONTROLLER_H
