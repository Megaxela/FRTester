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
    m_testingExecutor = std::make_shared<TestingExecutor>();
}

UnitTestsController::~UnitTestsController()
{

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
    QString path = QFileDialog::getSaveFileName(m_parent, "Сохранить", "", "Text (*.txt)");

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

# error ДОДЕЛАЙ РЕАЛИЗАЦИЮ ТВАРЬ
}
