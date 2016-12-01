//
// Created by megaxela on 22.11.16.
//

#include <include/Testing/TestCore.h>
#include <Tools/Logger.h>
#include "include/Windows/Controllers/TestControllers/UnitTestsController.h"
#include <QPushButton>
#include <include/Executor/TestingExecutor.h>
#include <QtWidgets/QFileDialog>

UnitTestsController::UnitTestsController(Ui::MainWindow *ptr, QWidget *parent) :
    m_ui(ptr),
    m_parent(parent)
{
    m_testingExecutor = new TestingExecutor();
}

UnitTestsController::~UnitTestsController()
{
    delete m_testingExecutor;
}

void UnitTestsController::setupConnections()
{
    // Кнопка обновления набора юнит тестов
    connect(m_ui->unitTestsUpdateTestsPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onUnitTestUpdateButtonPressed);

    // Кнопка запуска/остановки обработки тестов
    connect(m_ui->unitTestsStartStopPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onTestingStartedStoppedButtonPressed);

    // Кнопка паузы/продолжения обработки тестов
    connect(m_ui->unitTestsPauseResumePushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onTestingPausedResumedButtonPressed);

    // Кнопка сохранения лога
    connect(m_ui->unitTestsSaveLogPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onSavingLogButtonPressed);

    // Кнопка очистки лога
    connect(m_ui->unitTestsClearLogPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onClearLogButtonPressed);

    connect(m_testingExecutor,
            &TestingExecutor::testingFailed,
            this,
            &UnitTestsController::onTestingFailed);

    connect(m_testingExecutor,
            &TestingExecutor::testingStopped,
            this,
            &UnitTestsController::onTestingStopped
    );

    connect(m_testingExecutor,
            &TestingExecutor::testingFinished,
            this,
            &UnitTestsController::onTestingFinished
    );

    connect(m_testingExecutor,
            &TestingExecutor::testingPaused,
            this,
            &UnitTestsController::onTestingPaused
    );

    connect(m_testingExecutor,
            &TestingExecutor::testingResumed,
            this,
            &UnitTestsController::onTestingResumed
    );

    connect(m_testingExecutor,
            &TestingExecutor::testingStarted,
            this,
            &UnitTestsController::onTestingStarted
    );

    connect(m_testingExecutor,
            &TestingExecutor::testResultAcquired,
            this,
            &UnitTestsController::onTestingTestResultAcquired
    );

    connect(m_testingExecutor,
            &TestingExecutor::triggerFailAcquired,
            this,
            &UnitTestsController::onTestingTriggerFailAcquired
    );

    connect(m_testingExecutor,
            &TestingExecutor::testingLogAcquired,
            this,
            &UnitTestsController::onTestingLogAcquired
    );
}

void UnitTestsController::configureWidgets()
{

}

void UnitTestsController::tabSelected()
{

}

void UnitTestsController::updateUnitTestsSet()
{
    Log("Обновление набора тестов.")
    m_ui->unitTestsTreeWidget->clearTests();

    TestCore::instance().updateDatabase();

    auto tests = TestCore::instance().getTests();
    for (auto test : tests)
    {
        m_ui->unitTestsTreeWidget->addTest(test);
    }

    auto triggers = TestCore::instance().getTriggers();
    for (auto trigger : triggers)
    {
        m_ui->unitTestsTreeWidget->addTrigger(trigger);
    }
}

void UnitTestsController::onUnitTestUpdateButtonPressed()
{
    updateUnitTestsSet();
}

void UnitTestsController::onTestingStartedStoppedButtonPressed()
{
    if (m_testingExecutor->isTestingRunning())
    {
        m_testingExecutor->stop();
        m_ui->unitTestsStartStopPushButton->setText("Начать");
        m_ui->unitTestsPauseResumePushButton->setEnabled(false);
    }
    else
    {
        m_testingExecutor->start();
        m_ui->unitTestsStartStopPushButton->setText("Закончить");
        m_ui->unitTestsPauseResumePushButton->setEnabled(true);
    }
    m_ui->unitTestsPauseResumePushButton->setText("Приостановить");
}

void UnitTestsController::onTestingPausedResumedButtonPressed()
{
    if (m_testingExecutor->isPaused())
    {
        m_testingExecutor->resume();
        m_ui->unitTestsPauseResumePushButton->setText("Приостановить");
    }
    else
    {
        m_testingExecutor->pause();
        m_ui->unitTestsPauseResumePushButton->setText("Продолжить");
    }
}

void UnitTestsController::onSavingLogButtonPressed()
{
    QString path = QFileDialog::getSaveFileName(m_parent, "Сохранить", "", "Text (*.txt, *.html)");

    if (path.isEmpty())
    {
        return;
    }

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly))
    {
        Error("Не удалось сохранить лог из за того, что не удалось открыть файл " + path.toStdString());
        return;
    }

    QString logContent;

    if (path.endsWith(".html"))
    {
        logContent = m_ui->unitTestsLogTextEdit->toHtml();
    }
    else
    {
        logContent = m_ui->unitTestsLogTextEdit->toPlainText();
    }

    file.write(logContent.toUtf8());

    file.close();
}

void UnitTestsController::onClearLogButtonPressed()
{
    m_ui->unitTestsLogTextEdit->clear();
}

void UnitTestsController::onTestingFailed(QString reason)
{
    addLogMessage(
            "Тестирование было прервано, причина: " + reason,
            MessageType::Critical
    );
    onTestingStopped();
}

void UnitTestsController::addLogMessage(QString message,
                                        UnitTestsController::MessageType type)
{
    QColor color;

    switch (type)
    {
        case MessageType::Log:
            color = QColor(255, 255, 255);
            break;
        case MessageType::Error:
            color = QColor(170, 0, 0);
            break;
        case MessageType::Good:
            color = QColor(0, 170, 0);
            break;
        case MessageType::Bad:
            color = QColor(170, 0, 0);
            break;
        case MessageType::Critical:
            color = QColor(250, 0, 0);
            break;
    }

    Log("TEST: " + message.toStdString());

    m_ui->unitTestsLogTextEdit->insertHtml(
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:" +
        color.name() +
        ";\">" + message + "</span></p><br>"
    );
}

void UnitTestsController::onTestingStopped()
{
    addLogMessage("Тестирование было остановлено.");
    m_ui->unitTestsStartStopPushButton->setText("Начать");
    m_ui->unitTestsPauseResumePushButton->setText("Приостановить");
}

void UnitTestsController::onTestingFinished()
{
    addLogMessage("Тесетирование было завершено.");
    m_ui->unitTestsStartStopPushButton->setText("Начать");
    m_ui->unitTestsPauseResumePushButton->setText("Приостановить");
}

void UnitTestsController::onTestingPaused()
{
    addLogMessage("Тестирование приостановлено.");
    m_ui->unitTestsPauseResumePushButton->setText("Продолжить");
}

void UnitTestsController::onTestingResumed()
{
    addLogMessage("Тестирование возобнавлено.");
    m_ui->unitTestsPauseResumePushButton->setText("Приостановить");

}

void UnitTestsController::onTestingStarted()
{
    addLogMessage("Тестирование начато");
    m_ui->unitTestsStartStopPushButton->setText("Закончить");
}

void UnitTestsController::onTestingTestResultAcquired(TestPtr test, bool result)
{
    if (result)
    {
        addLogMessage(
                "Тест \"" + QString::fromStdString(test->name()) + "\" прошел успешно.",
                MessageType::Good
        );
    }
    else
    {
        addLogMessage(
                "Тест \"" + QString::fromStdString(test->name()) + "\" был провален.",
                MessageType::Bad
        );
    }
}

void UnitTestsController::onTestingTriggerFailAcquired(TriggerTestPtr trigger)
{
    addLogMessage(
            "Триггер \"" + QString::fromStdString(trigger->name()) + " был провален.",
            MessageType::Bad
    );
}

void UnitTestsController::onTestingLogAcquired(QString log)
{
    addLogMessage(log);
}
