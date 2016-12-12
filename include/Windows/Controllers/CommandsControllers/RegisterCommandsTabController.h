//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H
#define FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <ui_mainwindow.h>
#include <Windows/Controllers/CommandsTabController.h>

/**
 * @brief Контроллер работающий со вкладкой с регистрацией.
 */
class RegisterCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Укзатаель на UI.
     * @param parent Указатель на родительский виджет.
     */
    RegisterCommandsTabController (Ui::MainWindow *ptr,
                                   QWidget* parent);

    /**
     * @brief Метод для получения родителя - контроллера команд.
     * @return Указатель на контроллер.
     */
    CommandsTabController* commandsTabControllerParent() const;

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку продажи.
     */
    void onSellButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку покупки.
     */
    void onBuyButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку возврата продажи.
     */
    void onSellReturnButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку позврата прокупки.
     */
    void onBuyReturnButtonPressed();

    /**
     * @brief Слот, вызываемый при закрытии чека.
     */
    void onCheckClose();

    /**
     * @brief Слот, вызываемый при отмене чека.
     */
    void onCheckCancel();
};


#endif //FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H
