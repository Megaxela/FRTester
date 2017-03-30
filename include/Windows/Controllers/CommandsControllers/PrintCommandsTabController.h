//
// Created by megaxela on 07.12.16.
//

#ifndef FRTESTER_PRINTCOMMANDSTABCONTROLLER_H
#define FRTESTER_PRINTCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <QMap>

class CommandsTabController;

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс, контроллирующий вкладку c командами печати.
 */
class PrintCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     * @param parentController Указатель на родительский контроллер.
     */
    PrintCommandsTabController(Ui::MainWindow* ptr,
                               QWidget* parent,
                               QTabWidget* tabWidget);

    /**
     * @brief Деструктор.
     */
    ~PrintCommandsTabController();

protected:
    /**
     * @brief Метод для настройки соединений.
     */
    void setupConnections() override;

    /**
     * @brief Метод для настройки виджетов.
     */
    void configureWidgets() override;

};


#endif //FRTESTER_PRINTCOMMANDSTABCONTROLLER_H
