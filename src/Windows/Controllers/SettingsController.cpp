//
// Created by megaxela on 05.12.16.
//

#include <QFileDialog>
#include <Testing/SettingsSystem.h>
#include "Windows/Controllers/SettingsController.h"
#include "ui_mainwindow.h"

SettingsController::SettingsController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr)
{

}

SettingsController::~SettingsController()
{

}

void SettingsController::tabSelected()
{
    ui()->settingsTestsPathLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::TestsSharedTestsPath,
                            "tests/shared"
                    )
            )
    );

    ui()->settingsTriggersPathLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::TestsSharedTriggersPath,
                            "triggers/shared"
                    )
            )
    );

    ui()->settingsManualTestsPathLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::TestsManualTestsPath,
                            "tests/manual"
                    )
            )
    );
}

void SettingsController::setupConnections()
{
    // Кнопка выбора пути до тестов
    connect(ui()->settingsTestsPathPushButton,
            &QPushButton::clicked,
            this,
            &SettingsController::onTestsPathPushButtonPressed);

    // Кнопка выбора пути до триггеров
    connect(ui()->settingsTriggersPathPushButton,
            &QPushButton::clicked,
            this,
            &SettingsController::onTriggersPathPushButtonPressed);

    // Кнопка выбора пути до ручных тестов
    connect(ui()->settingsManualTestsPathPushButton,
            &QPushButton::clicked,
            this,
            &SettingsController::onManualTestsPathPushButtonPressed);
}

void SettingsController::configureWidgets()
{

}

void SettingsController::tabLeaved()
{
    SettingsSystem::instance().setValue(
            SettingsSystem::TestsSharedTestsPath,
            ui()->settingsTestsPathLineEdit->text().toStdString()
    );

    SettingsSystem::instance().setValue(
            SettingsSystem::TestsSharedTriggersPath,
            ui()->settingsTriggersPathLineEdit->text().toStdString()
    );

    SettingsSystem::instance().setValue(
            SettingsSystem::TestsManualTestsPath,
            ui()->settingsManualTestsPathLineEdit->text().toStdString()
    );
}

void SettingsController::onTestsPathPushButtonPressed()
{
    // Выбор пути
    QString path = QFileDialog::getExistingDirectory(
            parentWidget(),
            "Выберите папку с тестами.",
            ui()->settingsTestsPathLineEdit->text()
    );

    // Путь не был выбран
    if (path.isEmpty())
    {
        return;
    }

    ui()->settingsTestsPathLineEdit->setText(path);
}

void SettingsController::onTriggersPathPushButtonPressed()
{
    // Выбор пути
    QString path = QFileDialog::getExistingDirectory(
            parentWidget(),
            "Выберите папку с триггерами.",
            ui()->settingsTestsPathLineEdit->text()
    );

    // Путь не был выбран
    if (path.isEmpty())
    {
        return;
    }

    ui()->settingsTriggersPathLineEdit->setText(path);
}

void SettingsController::onManualTestsPathPushButtonPressed()
{
    // Выбор пути
    QString path = QFileDialog::getExistingDirectory(
            parentWidget(),
            "Выберите папку с ручными тестами.",
            ui()->settingsManualTestsPathLineEdit->text()
    );

    // Путь не был выбран
    if (path.isEmpty())
    {
        return;
    }

    ui()->settingsManualTestsPathLineEdit->setText(path);
}
