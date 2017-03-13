//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_UNITTESTSCONTROLLER_H
#define FRTESTER_UNITTESTSCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <include/Executor/TestingExecutor.h>
#include "ui_mainwindow.h"

class TestLoggerWaiter;

/**
 * @brief Контроллер, управляющий вкладкой с тестированием.
 */
class UnitTestsController : public AbstractTabController
{
    Q_OBJECT

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

    bool messageQuestionResult() const;

public slots:
    void openMessageQuestion(QString mess, QString ok, QString fl);

    void openMessageNotify(QString mess);

signals:

    void openMessageQuestionSignal(QString mess, QString ok, QString fl);

    void openMessageNotifySignal(QString mess);

    void onMessageBoxClosed();

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
     * @brief Слот, использующийся для
     * перерисовки окна со статистикой.
     */
    void redrawStatistics();

    /**
     * @brief Слот, вызываемый при вызове команды.
     * @param command Команды.
     */
    void onCommandCalled(QString command);

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

    /**
     * @brief Слот, вызываемый при выборе теста. Он занимается
     * отрисовкой переменных в тестах.
     * @param test Объект теста.
     */
    void onTestSelected(TestPtr test);

    /**
     * @brief Слот, вызываемый при выборе триггера. Он занимается
     * отрисовкой переменных в триггерах.
     * @param trigger Объект триггера.
     */
    void onTriggerSelected(TriggerTestPtr trigger);

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

    void onTestingErrorAcquired(QString log);

    // Слоты для TestLoggerWaiter
    void onTestLoggerWaiterLogAcquired(QString log);

    void onSelectedTestsExecuted(QVector<TestPtr> tests);

private:
    UnitTestsController(const UnitTestsController&) = delete;
    UnitTestsController& operator=(const UnitTestsController&) = delete;

    enum class MessageType
    {
        Log,
        Error,
        Good,
        Bad,
        Critical,
        TestMessage
    };

    void updateUnitTestsSet();

    void addLogMessage(QString message, MessageType type=MessageType::Log);

    void setTabSelectionEnabled(bool enabled);

    QString htmlScreening(QString source);

    bool m_questionResult;
    TestingExecutor* m_testingExecutor;
    TestLoggerWaiter* m_testLoggerWaiter;

    std::map<QString, std::pair<uint64_t, QTableWidgetItem*>> m_commandsCounter;
};


#endif //FRTESTER_UNITTESTSCONTROLLER_H
