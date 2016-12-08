//
// Created by megaxela on 07.12.16.
//

#ifndef FRTESTER_PRINTTEXTCOMMANDSTABCONTROLLER_H
#define FRTESTER_PRINTTEXTCOMMANDSTABCONTROLLER_H

#include <include/Windows/Controllers/AbstractTabController.h>
#include <QMap>

class CommandsTabController;

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс, контроллирующий вкладку с печатью текста.
 */
class PrintTextCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     * @param parentController Указатель на родительский контроллер.
     */
    PrintTextCommandsTabController(Ui::MainWindow* ptr,
                                   QWidget* parent,
                                   QTabWidget* parentController);

    /**
     * @brief Деструктор.
     */
    ~PrintTextCommandsTabController();

protected:
    void setupConnections() override;

    void configureWidgets() override;

};


#endif //FRTESTER_PRINTTEXTCOMMANDSTABCONTROLLER_H
