//
// Created by megaxela on 05.12.16.
//

#include <include/Tools/Settings.h>
#include <QtWidgets/QFileDialog>
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
    Settings::instance().load();

    ui()->settingsTestsPathLineEdit->setText(
            QString::fromStdString(
                    Settings::instance().getValue(
                            SETTINGS_NAMES_TESTSPATH,
                            "tests/"
                    )
            )
    );

    ui()->settingsTriggersPathLineEdit->setText(
            QString::fromStdString(
                    Settings::instance().getValue(
                            SETTINGS_NAMES_TRIGGERSPATH,
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
    Settings::instance().setValue(
            SETTINGS_NAMES_TESTSPATH,
            ui()->settingsTestsPathLineEdit->text().toStdString()
    );

    Settings::instance().setValue(
            SETTINGS_NAMES_TRIGGERSPATH,
            ui()->settingsTriggersPathLineEdit->text().toStdString()
    );

    Settings::instance().save();
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
