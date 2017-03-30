
//
// Created by megaxela on 26.12.16.
//

#ifndef FRTESTER_PRINTTEXTPLAINTEXTCOMMANDSTABCONTROLLER_H
#define FRTESTER_PRINTTEXTPLAINTEXTCOMMANDSTABCONTROLLER_H

#include <Windows/Controllers/AbstractTabController.h>
#include <Windows/Controllers/CommandsTabController.h>

/**
 * @brief Контроллер, работающий со вкладкой печати простого текста.
 */
class PrintTextPlainTextCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     */
    PrintTextPlainTextCommandsTabController(Ui::MainWindow *ptr, QWidget *parent);

protected:
    void setupConnections() override;

    void configureWidgets() override;


private slots:
    /**
     * @brief Слот, вызываемый при нажатии на
     * кнопку печати
     */
    void onPrintButtonPressed();

private:
    /**
     * @brief Метод для получения родительского контроллера команд.
     * @return Указатель на родительский контроллер.
     */
    CommandsTabController* commandsTabControllerParent() const;

    /**
     * @brief Метод для обработки входных данных.
     * @param str Строка.
     * @return Результат.
     */
    void parseInput(const QString &str, std::vector<std::string> &lines);
};


#endif //FRTESTER_PRINTTEXTPLAINTEXTCOMMANDSTABCONTROLLER_H
