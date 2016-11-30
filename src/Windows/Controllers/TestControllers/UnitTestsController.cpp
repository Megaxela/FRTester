//
// Created by megaxela on 22.11.16.
//

#include <include/Testing/TestCore.h>
#include <Tools/Logger.h>
#include "UnitTestsController.h"

UnitTestsController::UnitTestsController(Ui::MainWindow *ptr, QWidget *parent) :
    m_ui(ptr),
    m_parent(parent)
{

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
            &UnitTestsController::onUnitTestUpdate);
}

void UnitTestsController::configureWidgets()
{

}

void UnitTestsController::tabSelected()
{

}

void UnitTestsController::onUnitTestUpdate()
{

}

void UnitTestsController::onTestingStarted()
{

}

void UnitTestsController::onTestingStopped()
{

}

void UnitTestsController::updateUnitTestsSet()
{
    Log("Обновление набора тестов.")
    TestCore::instance().updateDatabase();

    m_ui->unitTestsListWidget->clear();

    auto tests = TestCore::instance().getTests();
    for (auto test : tests)
    {
        m_ui->unitTestsListWidget->addItem(
                QString::fromStdString(
                        test->name()
                )
        );
    }
}
