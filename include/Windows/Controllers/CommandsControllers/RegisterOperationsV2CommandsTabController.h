//
// Created by megaxela on 15.03.17.
//

#ifndef FRTESTER_REGISTEROPERATIONSV2COMMANDSTABCONTROLLER_H
#define FRTESTER_REGISTEROPERATIONSV2COMMANDSTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>
#include <include/Windows/Controllers/CommandsTabController.h>

/**
 * @brief Контроллер работающий со вкладкой с регистрацией с
 * испольхованием операций V2.
 */
class RegisterOperationsV2CommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Укзатаель на UI.
     * @param parent Указатель на родительский виджет.
     */
    RegisterOperationsV2CommandsTabController(
            Ui::MainWindow* ptr,
            QWidget* parent
    );

    /**
     * @brief Метод для получения родителя - контроллера команд.
     * @return Указатель на контроллер.
     */
    CommandsTabController* commandsTabController() const;

protected:
    virtual void setupConnections();

    virtual void configureWidgets();

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку покупки.
     */
    void onBuyButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку продажи.
     */
    void onSellButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку возврата продажи.
     */
    void onReturnBuyButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку возврата покупки.
     */
    void onReturnSellButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку продолжения печати.
     */
    void onResumePrintingButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку аннулирования чека.
     */
    void onCheckCancelButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку закрытия чека.
     */
    void onCheckCloseButtonPressed();

private:

    uint64_t performOperation(FRDriver::OperationType operationType);

};


#endif //FRTESTER_REGISTEROPERATIONSV2COMMANDSTABCONTROLLER_H
