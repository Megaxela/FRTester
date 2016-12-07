//
// Created by megaxela on 07.12.16.
//

#ifndef FRTESTER_PRINTTEXTOPERATIONSCOMMANDSTABCONTROLLER_H
#define FRTESTER_PRINTTEXTOPERATIONSCOMMANDSTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>

class CommandsTabController;

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс, контроллирующий вкладку с операциями печати теста.
 */
class PrintTextOperationsCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     * @param parentController Указатель на родительский контроллер.
     */
    PrintTextOperationsCommandsTabController(Ui::MainWindow* ptr,
                                             QWidget* parent,
                                             AbstractTabController* parentController);

    /**
     * @brief Деструктор.
     */
    ~PrintTextOperationsCommandsTabController();

protected:
    /**
     * @brief Метод для настройки соединений сигналов со слотами.
     */
    void setupConnections() override;

    /**
     * @brief Метод для настройки виджетов.
     */
    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку продолжения
     * печати.
     */
    void onContinuePrintPressed();

private:
    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    CommandsTabController* m_parentController;
};


#endif //FRTESTER_PRINTTEXTOPERATIONSCOMMANDSTABCONTROLLER_H
