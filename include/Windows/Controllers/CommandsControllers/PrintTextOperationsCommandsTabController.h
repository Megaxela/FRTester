//
// Created by megaxela on 07.12.16.
//

#ifndef FRTESTER_PRINTTEXTOPERATIONSCOMMANDSTABCONTROLLER_H
#define FRTESTER_PRINTTEXTOPERATIONSCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

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
                                             QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~PrintTextOperationsCommandsTabController();

    /**
     * @brief Метод для получения родителя - контроллера команд.
     * @return Указатель на контроллер.
     */
    CommandsTabController* commandsTabControllerParent() const;

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

    /**
     * @brief Слот, вызываемый при нажатии на кнопку обрезки ленты.
     */
    void onCutPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку прокрутки ленты.
     */
    void onScrollPressed();
};


#endif //FRTESTER_PRINTTEXTOPERATIONSCOMMANDSTABCONTROLLER_H
