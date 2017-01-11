//
// Created by megaxela on 22.11.16.
//

#include <Testing/TestCore.h>
#include <Tools/Logger.h>
#include "Windows/Controllers/TestControllers/UnitTestsController.h"
#include "Executor/TestLoggerWaiter.h"
#include <QFileDialog>

UnitTestsController::UnitTestsController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr)
{
    m_testingExecutor = new TestingExecutor();
    m_testLoggerWaiter = new TestLoggerWaiter();
    m_testLoggerWaiter->start();
}

UnitTestsController::~UnitTestsController()
{
    delete m_testingExecutor;
}

void UnitTestsController::setupConnections()
{
    // Кнопка обновления набора юнит тестов
    connect(ui()->unitTestsUpdateTestsPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onUnitTestUpdateButtonPressed);

    // Кнопка запуска/остановки обработки тестов
    connect(ui()->unitTestsStartStopPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onTestingStartedStoppedButtonPressed);

    // Кнопка паузы/продолжения обработки тестов
    connect(ui()->unitTestsPauseResumePushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onTestingPausedResumedButtonPressed);

    // Кнопка сохранения лога
    connect(ui()->unitTestsSaveLogPushButton,
            &QPushButton::clicked,
            this,
            &UnitTestsController::onSavingLogButtonPressed);

    // Кнопка очистки лога
    connect(ui()->unitTestsClearLogPushButton,
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

    connect(m_testingExecutor,
            &TestingExecutor::testingErrorAcquired,
            this,
            &UnitTestsController::onTestingErrorAcquired);

    connect(m_testLoggerWaiter,
            &TestLoggerWaiter::logReceived,
            this,
            &UnitTestsController::onTestLoggerWaiterLogAcquired);

    connect(ui()->unitTestsTreeWidget,
            &QTestsTreeWidget::selectedTestsExecuted,
            this,
            &UnitTestsController::onSelectedTestsExecuted);
}

void UnitTestsController::configureWidgets()
{
    int id = QFontDatabase::addApplicationFont(":/files/fonts/FiraCode-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont font(family);

    ui()->unitTestsLogTextEdit->setFont(font);
    ui()->unitTestsLogTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui()->unitTestsLogTextEdit->setUndoRedoEnabled(false);
    ui()->unitTestsLogTextEdit->setReadOnly(true);
}

void UnitTestsController::tabSelected()
{
    updateUnitTestsSet();
}

void UnitTestsController::updateUnitTestsSet()
{
    Log("Обновление набора тестов.")
    ui()->unitTestsTreeWidget->clearTests();

    TestCore::instance().updateDatabase();

    auto tests = TestCore::instance().getTests();
    for (auto test : tests)
    {
        ui()->unitTestsTreeWidget->addTest(test);
    }

    auto triggers = TestCore::instance().getTriggers();
    for (auto trigger : triggers)
    {
        ui()->unitTestsTreeWidget->addTrigger(trigger);
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
        ui()->unitTestsStartStopPushButton->setText("Начать");
        ui()->unitTestsPauseResumePushButton->setEnabled(false);
    }
    else
    {
        m_testingExecutor->start();
        ui()->unitTestsStartStopPushButton->setText("Закончить");
        ui()->unitTestsPauseResumePushButton->setEnabled(true);
    }
    ui()->unitTestsPauseResumePushButton->setText("Приостановить");
}

void UnitTestsController::onTestingPausedResumedButtonPressed()
{
    if (m_testingExecutor->isPaused())
    {
        m_testingExecutor->resume();
//        ui()->unitTestsPauseResumePushButton->setText("Приостановить");
    }
    else
    {
        m_testingExecutor->pause();
//        ui()->unitTestsPauseResumePushButton->setText("Продолжить");
    }
}

void UnitTestsController::onSavingLogButtonPressed()
{
    QString path = QFileDialog::getSaveFileName(
            parentWidget(), 
            "Сохранить", 
            "", 
            "Text (*.txt, *.html)"
    );

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
        logContent = ui()->unitTestsLogTextEdit->toHtml();
    }
    else
    {
        logContent = ui()->unitTestsLogTextEdit->toPlainText();
    }

    file.write(logContent.toUtf8());

    file.close();
}

void UnitTestsController::onClearLogButtonPressed()
{
    ui()->unitTestsLogTextEdit->clear();
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
            color = QColor(0xff, 0xff, 0xff);
            break;
        case MessageType::TestMessage:
            color = QColor(0xff, 0xcc, 0x00);
            break;
        case MessageType::Error:
            color = QColor(0xaa, 0, 0);
            break;
        case MessageType::Good:
            color = QColor(0, 0xaa, 0);
            break;
        case MessageType::Bad:
            color = QColor(0xaa, 0, 0);
            break;
        case MessageType::Critical:
            color = QColor(0xfa, 0, 0);
            break;
    }

    Log("TEST: " + message.toStdString());

    auto textCursor = ui()->unitTestsLogTextEdit->textCursor();

    ui()->unitTestsLogTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

    ui()->unitTestsLogTextEdit->append(
        "<span style=\" color:" +
        color.name() +
        ";\">" + htmlScreening(message) + "</span>"
    );
}

void UnitTestsController::onTestingStopped()
{
    addLogMessage("Тестирование было остановлено.");
    ui()->unitTestsStartStopPushButton->setText("Начать");
    ui()->unitTestsPauseResumePushButton->setText("Приостановить");
    ui()->unitTestsPauseResumePushButton->setEnabled(false);
}

void UnitTestsController::onTestingFinished()
{
    addLogMessage("Тестирование было завершено.");
    ui()->unitTestsStartStopPushButton->setText("Начать");
    ui()->unitTestsPauseResumePushButton->setText("Приостановить");
    ui()->unitTestsPauseResumePushButton->setEnabled(false);
}

void UnitTestsController::onTestingPaused()
{
    addLogMessage("Тестирование приостановлено.");
    ui()->unitTestsPauseResumePushButton->setText("Продолжить");
}

void UnitTestsController::onTestingResumed()
{
    addLogMessage("Тестирование возобнавлено.");
    ui()->unitTestsPauseResumePushButton->setText("Приостановить");

}

void UnitTestsController::onTestingStarted()
{
    onClearLogButtonPressed();
    addLogMessage("Тестирование начато");
    ui()->unitTestsStartStopPushButton->setText("Закончить");
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

void UnitTestsController::onTestingErrorAcquired(QString log)
{
    addLogMessage(log, MessageType::Critical);
}

void UnitTestsController::onTestLoggerWaiterLogAcquired(QString log)
{
    addLogMessage("[ТЕСТ]-> " + log, MessageType::TestMessage);
}

QString UnitTestsController::htmlScreening(QString source)
{
    QString result;

    for (auto ch : source)
    {
        if (ch == QChar('<'))
        {
            result.append("&lt;");
        }
        else if (ch == QChar('>'))
        {
            result.append("&gt;");
        }
        else if (ch == QChar('"'))
        {
            result.append("&quot;");
        }
        else if (ch == QChar(' '))
        {
            result.append("&nbsp;");
        }
        else if (ch == QChar('\''))
        {
            result.append("&#39;");
        }
        else if (ch == QChar('\n'))
        {
            result.append("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
        }
        else if (ch == QChar('&'))
        {
            result.append("&amp;");
        }
        else
        {
            result.append(ch);
        }
    }

    return result;
}

void UnitTestsController::onSelectedTestsExecuted(QVector<TestPtr> tests)
{
    m_testingExecutor->setTestsToRun(tests.toStdVector());
}
