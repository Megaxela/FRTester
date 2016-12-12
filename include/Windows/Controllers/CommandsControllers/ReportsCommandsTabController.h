//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H
#define FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H

#include <include/Windows/Controllers/AbstractTabController.h>
#include <ui_mainwindow.h>
#include <include/Windows/Controllers/CommandsTabController.h>

/**
 * @brief Контроллер, управляющий вкладкой с очетами.
 */
class ReportsCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на графический интерфейс.
     * @param parent Указатель на родительский виджет
     */
    ReportsCommandsTabController(Ui::MainWindow* ptr,
                                 QWidget* parent);

    /**
     * @brief Метод для получения родительского контроллера
     * вкладки с коммандами.
     * @return Родительский контроллер с коммандами.
     */
    CommandsTabController* commandsTabControllerParent() const;

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    void onZReportPressed();

    void onChangeOpenPressed();

    void onXReportPressed();

    void onDepartmentReportPressed();

    void onTaxesReportPressed();

    void onCashiersReportPressed();
};


#endif //FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H
