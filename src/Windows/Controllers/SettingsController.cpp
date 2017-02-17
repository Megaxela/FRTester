//
// Created by megaxela on 05.12.16.
//

#include <QtWidgets/QFileDialog>
#include <include/Testing/SettingsSystem.h>
#include "include/Windows/Controllers/SettingsController.h"
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
                            "tests/"
                    )
            )
    );

    ui()->settingsTriggersPathLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::TestsSharedTriggersPath,
                            "triggers/"
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
