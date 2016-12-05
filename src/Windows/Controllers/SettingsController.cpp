//
// Created by megaxela on 05.12.16.
//

#include <include/Tools/Settings.h>
#include <QtWidgets/QFileDialog>
#include "include/Windows/Controllers/SettingsController.h"
#include "ui_mainwindow.h"

SettingsController::SettingsController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(),
    m_ui(ptr),
    m_parent(parent)
{

}

SettingsController::~SettingsController()
{

}

void SettingsController::tabSelected()
{
    Settings::instance().load();

    m_ui->settingsTestsPathLineEdit->setText(
            QString::fromStdString(
                    Settings::instance().getValue(
                            SETTINGS_NAMES_TESTSPATH,
                            "tests/"
                    )
            )
    );

    m_ui->settingsTriggersPathLineEdit->setText(
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
    connect(m_ui->settingsTestsPathPushButton,
            &QPushButton::clicked,
            this,
            &SettingsController::onTestsPathPushButtonPressed);

    // Кнопка выбора пути до триггеров
    connect(m_ui->settingsTriggersPathPushButton,
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
            m_ui->settingsTestsPathLineEdit->text().toStdString()
    );

    Settings::instance().setValue(
            SETTINGS_NAMES_TRIGGERSPATH,
            m_ui->settingsTriggersPathLineEdit->text().toStdString()
    );

    Settings::instance().save();
}

void SettingsController::onTestsPathPushButtonPressed()
{
    // Выбор пути
    QString path = QFileDialog::getExistingDirectory(
            m_parent,
            "Выберите папку с тестами.",
            m_ui->settingsTestsPathLineEdit->text()
    );

    // Путь не был выбран
    if (path.isEmpty())
    {
        return;
    }

    m_ui->settingsTestsPathLineEdit->setText(path);
}

void SettingsController::onTriggersPathPushButtonPressed()
{
    // Выбор пути
    QString path = QFileDialog::getExistingDirectory(
            m_parent,
            "Выберите папку с триггерами.",
            m_ui->settingsTestsPathLineEdit->text()
    );

    // Путь не был выбран
    if (path.isEmpty())
    {
        return;
    }

    m_ui->settingsTriggersPathLineEdit->setText(path);
}
