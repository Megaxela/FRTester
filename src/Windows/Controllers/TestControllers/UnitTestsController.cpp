//
// Created by megaxela on 22.11.16.
//

#include <Testing/TestCore.h>
#include <Tools/Logger.h>
#include "Windows/Controllers/TestControllers/UnitTestsController.h"
#include "Executor/TestLoggerWaiter.h"
#include <QFileDialog>
#include <Windows/Widgets/QNumberLineEdit.h>
#include <QCheckBox>
#include <TestDriverHolder.h>
#include <Tools/GUIHelper.h>
#include <QMessageBox>
#include <include/Testing/SettingsSystem.h>
#include <Windows/Widgets/QHexLineEdit.h>

UnitTestsController::UnitTestsController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr),
    m_questionResult(false),
    m_testingExecutor(new TestingExecutor()),
    m_testLoggerWaiter(new TestLoggerWaiter()),
    m_commandsCounter()
{
    TestCore::instance().setTestExecutor(m_testingExecutor);
    TestCore::instance().setUnitTestsController(this);

    m_testLoggerWaiter->start();

    connect(this,
            &UnitTestsController::openMessageNotifySignal,
            this,
            &UnitTestsController::openMessageNotify);
    connect(this,
            &UnitTestsController::openMessageQuestionSignal,
            this,
            &UnitTestsController::openMessageQuestion);

    ui()->unitTestsRestorePasswordLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::TestsRestoreStatePassword,
                            "30"
                    )
            )
    );

    auto formLayout = ui()->unitTestsTestValuesFormLayout;
    QLayoutItem* child;
    while ((child = formLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }
    updateUnitTestsSet();
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

    // Выбор теста
    connect(ui()->unitTestsTreeWidget,
            &QTestsTreeWidget::testSelected,
            this,
            &UnitTestsController::onTestSelected);

    connect(ui()->unitTestsTreeWidget,
            &QTestsTreeWidget::triggerSelected,
            this,
            &UnitTestsController::onTriggerSelected);

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

    connect(m_testingExecutor,
            &TestingExecutor::commandCalled,
            this,
            &UnitTestsController::onCommandCalled);

    connect(m_testLoggerWaiter,
            &TestLoggerWaiter::logReceived,
            this,
            &UnitTestsController::onTestLoggerWaiterLogAcquired);

    connect(ui()->unitTestsTreeWidget,
            &QTestsTreeWidget::selectedTestsExecuted,
            this,
            &UnitTestsController::onSelectedTestsExecuted);

    connect(ui()->unitTestsRestorePasswordLineEdit,
            &QLineEdit::editingFinished,
            this,
            &UnitTestsController::onRestorePasswordChanged);
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

    ui()->unitTestsStatisticsTableWidget->verticalHeader()->show();
}

void UnitTestsController::tabSelected()
{

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
    setTabSelectionEnabled(true);
}

void UnitTestsController::onTestingFinished()
{
    // Выводим статистику в лог по просьбе Прохора
    Log("=============STATISTIC=============")
    for (int i = 0; i < ui()->unitTestsStatisticsTableWidget->rowCount(); ++i)
    {
        Log("[" +
            ui()->unitTestsStatisticsTableWidget->item(i, 0)->text().toStdString() +
            "](" +
            ui()->unitTestsStatisticsTableWidget->item(i, 1)->text().toStdString() +
            ")"
        );
    }
    Log("===================================");

    addLogMessage("Тестирование было завершено.");
    ui()->unitTestsStartStopPushButton->setText("Начать");
    ui()->unitTestsPauseResumePushButton->setText("Приостановить");
    ui()->unitTestsPauseResumePushButton->setEnabled(false);
    setTabSelectionEnabled(true);
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
    ui()->unitTestsStatisticsTableWidget->clear();
    m_commandsCounter.clear();
    ui()->unitTestsStatisticsTableWidget->setRowCount(0);
    onClearLogButtonPressed();
    addLogMessage("Тестирование начато");
    ui()->unitTestsStartStopPushButton->setText("Закончить");
    setTabSelectionEnabled(false);
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
    addLogMessage("-> " + log, MessageType::TestMessage);
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

void UnitTestsController::onTestSelected(TestPtr test)
{
    Log("Выбран тест \"" + test->name() + "\".");

    // Загружаем переменные и отрисовываем их в formLayout
    auto variables = test->getVariables();

    // Получение formLayout
    auto formLayout = ui()->unitTestsTestValuesFormLayout;

    // Очистка FormLayout
    GUIHelper::cleanFormLayout(formLayout);

    // Отрисовка новых виджетов
    for (auto& variable : variables)
    {
        QWidget* widget = nullptr;

        // Выбор виджета для типа данных
        switch (variable.second)
        {
        case DataValue::Type::UInt8:
        {
            auto numberLineEdit = new QNumberLineEdit<uint8_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toUInt8());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint8_t>::newUInt8ValueAccepted,
                    [=](uint8_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int8:
        {
            auto numberLineEdit = new QNumberLineEdit<int8_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toInt8());
            connect(numberLineEdit,
                    &QNumberLineEdit<int8_t>::newInt8ValueAccepted,
                    [=](int8_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt16:
        {
            auto numberLineEdit = new QNumberLineEdit<uint16_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toUInt16());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint16_t>::newUInt16ValueAccepted,
                    [=](uint16_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int16:
        {
            auto numberLineEdit = new QNumberLineEdit<int16_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toInt16());
            connect(numberLineEdit,
                    &QNumberLineEdit<int16_t>::newInt8ValueAccepted,
                    [=](int16_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt32:
        {
            auto numberLineEdit = new QNumberLineEdit<uint32_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toUInt32());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint32_t>::newUInt32ValueAccepted,
                    [=](uint32_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int32:
        {
            auto numberLineEdit = new QNumberLineEdit<int32_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toInt32());
            connect(numberLineEdit,
                    &QNumberLineEdit<int32_t>::newInt32ValueAccepted,
                    [=](int32_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt64:
        {
            auto numberLineEdit = new QNumberLineEdit<uint64_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toUInt64());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint64_t>::newUInt64ValueAccepted,
                    [=](uint64_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int64:
        {
            auto numberLineEdit = new QNumberLineEdit<int64_t>(parentWidget());
            numberLineEdit->setNumberValue(test->getValue(variable.first).toInt64());
            connect(numberLineEdit,
                    &QNumberLineEdit<int64_t>::newInt64ValueAccepted,
                    [=](int64_t value)
                    {
                        test->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Boolean:
        {
            auto checkBox = new QCheckBox(parentWidget());
            checkBox->setChecked(test->getValue(variable.first).toBoolean());
            connect(checkBox,
                    &QCheckBox::stateChanged,
                    [=](int state)
                    {
                        test->setValue(variable.first, (bool) state);
                    });

            widget = checkBox;
            break;
        }
        case DataValue::Type::String:
        {
            auto lineEdit = new QLineEdit(parentWidget());
            lineEdit->setText(QString::fromStdString(test->getValue(variable.first).toString()));
            connect(lineEdit,
                    &QLineEdit::textEdited,
                    [=](QString s)
                    {
                        test->setValue(variable.first, s.toStdString());
                    });

            widget = lineEdit;
            break;
        }
        case DataValue::Type::ByteArray:
        {
            auto byteArray = new QHexLineEdit(parentWidget());
            byteArray->setByteArray(test->getValue(variable.first).toByteArray());
            connect(byteArray,
                    &QHexLineEdit::byteArrayChanged,
                    [=](const ByteArray &a)
                    {
                        test->setValue(variable.first, a);
                    });

            widget = byteArray;
            break;
        }

        }

        if (widget != nullptr)
        {
            formLayout->addRow(
                    QString::fromStdString(variable.first),
                    widget
            );
        }
    }
}

void UnitTestsController::onCommandCalled(QString tag)
{
    auto findIterator = m_commandsCounter.find(tag);

    if (findIterator != m_commandsCounter.end())
    {
        findIterator->second.first += 1;
    }
    else
    {
        m_commandsCounter[tag].first = 1;
        m_commandsCounter[tag].second = nullptr;
    }

    redrawStatistics();
}

void UnitTestsController::redrawStatistics()
{
    for (auto& element : m_commandsCounter)
    {
        if (element.second.second)
        {
            element.second.second->setText(QString::number(element.second.first));
        }
        else
        {
            // Add table element
            element.second.second = new QTableWidgetItem(
                    (int) element.second.first
            );

            ui()->unitTestsStatisticsTableWidget->setRowCount(
                    ui()->unitTestsStatisticsTableWidget->rowCount() + 1
            );

            ui()->unitTestsStatisticsTableWidget->setItem(
                    ui()->unitTestsStatisticsTableWidget->rowCount() - 1,
                    0,
                    new QTableWidgetItem(element.first)
            );

            ui()->unitTestsStatisticsTableWidget->setItem(
                    ui()->unitTestsStatisticsTableWidget->rowCount() - 1,
                    1,
                    element.second.second
            );
        }
    }
}

void UnitTestsController::onTriggerSelected(TriggerTestPtr trigger)
{
    Log("Выбран триггер \"" + trigger->name() + "\".");

    // Загружаем переменные и отрисовываем их в formLayout
    auto variables = trigger->getVariables();

    // Получение formLayout
    auto formLayout = ui()->unitTestsTestValuesFormLayout;

    // Очистка FormLayout
    QLayoutItem* child;
    while ((child = formLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    // Отрисовка новых виджетов
    for (auto& variable : variables)
    {
        QWidget* widget = nullptr;

        // Выбор виджета для типа данных
        switch (variable.second)
        {
        case DataValue::Type::UInt8:
        {
            auto numberLineEdit = new QNumberLineEdit<uint8_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toUInt8());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint8_t>::newUInt8ValueAccepted,
                    [=](uint8_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int8:
        {
            auto numberLineEdit = new QNumberLineEdit<int8_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toInt8());
            connect(numberLineEdit,
                    &QNumberLineEdit<int8_t>::newInt8ValueAccepted,
                    [=](int8_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt16:
        {
            auto numberLineEdit = new QNumberLineEdit<uint16_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toUInt16());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint16_t>::newUInt16ValueAccepted,
                    [=](uint16_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int16:
        {
            auto numberLineEdit = new QNumberLineEdit<int16_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toInt16());
            connect(numberLineEdit,
                    &QNumberLineEdit<int16_t>::newInt8ValueAccepted,
                    [=](int16_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt32:
        {
            auto numberLineEdit = new QNumberLineEdit<uint32_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toUInt32());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint32_t>::newUInt32ValueAccepted,
                    [=](uint32_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int32:
        {
            auto numberLineEdit = new QNumberLineEdit<int32_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toInt32());
            connect(numberLineEdit,
                    &QNumberLineEdit<int32_t>::newInt32ValueAccepted,
                    [=](int32_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::UInt64:
        {
            auto numberLineEdit = new QNumberLineEdit<uint64_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toUInt64());
            connect(numberLineEdit,
                    &QNumberLineEdit<uint64_t>::newUInt64ValueAccepted,
                    [=](uint64_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Int64:
        {
            auto numberLineEdit = new QNumberLineEdit<int64_t>(parentWidget());
            numberLineEdit->setNumberValue(trigger->getValue(variable.first).toInt64());
            connect(numberLineEdit,
                    &QNumberLineEdit<int64_t>::newInt64ValueAccepted,
                    [=](int64_t value)
                    {
                        trigger->setValue(variable.first, value);
                    });

            widget = numberLineEdit;
            break;
        }
        case DataValue::Type::Boolean:
        {
            auto checkBox = new QCheckBox(parentWidget());
            checkBox->setChecked(trigger->getValue(variable.first).toBoolean());
            connect(checkBox,
                    &QCheckBox::stateChanged,
                    [=](int state)
                    {
                        trigger->setValue(variable.first, (bool) state);
                    });

            widget = checkBox;
            break;
        }
        case DataValue::Type::String:
        {
            auto lineEdit = new QLineEdit(parentWidget());
            lineEdit->setText(QString::fromStdString(trigger->getValue(variable.first).toString()));
            connect(lineEdit,
                    &QLineEdit::textEdited,
                    [=](QString s)
                    {
                        trigger->setValue(variable.first, s.toStdString());
                    });

            widget = lineEdit;
            break;
        }
        case DataValue::Type::ByteArray:
        {
            auto byteArray = new QHexLineEdit(parentWidget());
            byteArray->setByteArray(trigger->getValue(variable.first).toByteArray());
            connect(byteArray,
                    &QHexLineEdit::byteArrayChanged,
                    [=](const ByteArray &a)
                    {
                        trigger->setValue(variable.first, a);
                    });

            widget = byteArray;
            break;
        }
        }

        if (widget != nullptr)
        {
            formLayout->addRow(
                    QString::fromStdString(variable.first),
                    widget
            );
        }
    }
}

void UnitTestsController::setTabSelectionEnabled(bool enabled)
{
    for (int i = 0; i < ui()->mainTabWidget->count(); ++i)
    {
        if (ui()->mainTabWidget->currentIndex() != i)
        {
            ui()->mainTabWidget->setTabEnabled(i, enabled);
        }
    }
}

void UnitTestsController::openMessageQuestion(QString mess, QString ok, QString fl)
{
    m_questionResult = QMessageBox::question(
            parentWidget(),
            "Вопрос от теста",
            mess,
            ok,
            fl
    ) == 0;

    emit onMessageBoxClosed();
}

void UnitTestsController::openMessageNotify(QString mess)
{
    QMessageBox::information(
            parentWidget(),
            "Информация",
            mess
    );

    emit onMessageBoxClosed();
}

bool UnitTestsController::messageQuestionResult() const
{
    return m_questionResult;
}

void UnitTestsController::onRestorePasswordChanged()
{
    bool ok = false;

    QString value = ui()->unitTestsRestorePasswordLineEdit->text();

    uint32_t pwd = value.toUInt(&ok);

    if (!ok)
    {
        ErrorStream() << value.toStdString() << " is not the 32bit number." << std::endl;
        return;
    }

    TestCore::instance().setRestorePassword(pwd);
}
