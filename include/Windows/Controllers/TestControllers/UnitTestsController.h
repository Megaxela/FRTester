//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_UNITTESTSCONTROLLER_H
#define FRTESTER_UNITTESTSCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <include/Executor/TestingExecutor.h>
#include "ui_mainwindow.h"

/**
 * @brief Контроллер, управляющий вкладкой с тестированием.
 */
class UnitTestsController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский объект.
     */
    UnitTestsController(Ui::MainWindow* ptr, QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~UnitTestsController() override;

protected:
    /**
     * @brief Метод для настройки коннектов.
     */
    void setupConnections() override;

    /**
     * @brief Метод для настройки виджетов.
     */
    void configureWidgets() override;

    /**
     * @brief Метод вызываемый при выборе вкладки.
     */
    void tabSelected() override;

private slots:

    /**
     * @brief Слот, вызываемый при обновлении
     * набора тестов.
     */
    void onUnitTestUpdateButtonPressed();

    /**
     * @brief Слот, вызываемый при запуске/остановке тестов.
     */
    void onTestingStartedStoppedButtonPressed();

    /**
     * @brief Слот, вызываемый при приостановке/продолжении тестирования.
     */
    void onTestingPausedResumedButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку сохранения лога.
     */
    void onSavingLogButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку очистки лога.
     */
    void onClearLogButtonPressed();

    // Слоты для TestingExecutor
    void onTestingFailed(QString reason);

    void onTestingStopped();

    void onTestingFinished();

    void onTestingPaused();

    void onTestingResumed();

    void onTestingStarted();

    void onTestingTestResultAcquired(TestPtr test, bool result);

    void onTestingTriggerFailAcquired(TriggerTestPtr trigger);

    void onTestingLogAcquired(QString log);

private:
    enum class MessageType
    {
        Log,
        Error,
        Good,
        Bad,
        Critical
    };

    void updateUnitTestsSet();

    void addLogMessage(QString message, MessageType type=MessageType::Log);

    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    TestingExecutor* m_testingExecutor;
};


#endif //FRTESTER_UNITTESTSCONTROLLER_H
