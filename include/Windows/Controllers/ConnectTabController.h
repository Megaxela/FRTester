//
// Created by megaxela on 08.12.16.
//

#ifndef FRTESTER_CONNECTTABCONTROLLER_H
#define FRTESTER_CONNECTTABCONTROLLER_H

#include <QWidget>
#include <QTabWidget>
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

protected:
    void setupConnections() override;

    void configureWidgets() override;
};


#endif //FRTESTER_CONNECTTABCONTROLLER_H
