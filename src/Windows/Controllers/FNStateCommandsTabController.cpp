//
// Created by megaxela on 31.03.17.
//

#include "include/Windows/Controllers/FNStateCommandsTabController.h"
#include <ui_mainwindow.h>
#include <Tools/Logger.h>
#include <Windows/Controllers/CommandsTabController.h>
#include <include/DriverHolder.h>

FNStateCommandsTabController::FNStateCommandsTabController(Ui::MainWindow *ptr, QWidget *parent)
        : AbstractTabController(ptr, parent, nullptr)
{

}

FNStateCommandsTabController::~FNStateCommandsTabController()
{

}

void FNStateCommandsTabController::setupConnections()
{
    connect(ui()->commandsFNStateGetStatePushButton,
            &QPushButton::clicked,
            this,
            &FNStateCommandsTabController::onGetFNState);
}

void FNStateCommandsTabController::configureWidgets()
{
    int id = QFontDatabase::addApplicationFont(":/files/fonts/FiraCode-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont font(family);

    ui()->commandsFNStateLogPlainTextEdit->setFont(font);
}

void FNStateCommandsTabController::onGetFNState()
{
    Log("Выполняем запрос статуса ФН");
    ExcessLog("Проверка наличия соединения");
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ExcessLog("Запрос на ККТ.");
    auto requestResult = DriverHolder::driver().getFNStatus(
        commandsTabController()->password()
    );

    ExcessLog("Установка статуса.");
    commandsTabController()->setLastStatus();

    ExcessLog("Проверка на успешность выполнения команды.");
    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        return;
    }

    QString stringBuilder;
    QString divider = "==================================================================";

    stringBuilder.append(divider + '\n');
    stringBuilder.append("Состояние фазы жизни\n");
    stringBuilder.append(divider + '\n');
    stringBuilder.append("    Проведена настройка ФН:    " + stringifyBool(requestResult.settedUp) + "\n");
    stringBuilder.append("    Открыт фискальный режим:   " + stringifyBool(requestResult.fiscalModeOpened) + "\n");
    stringBuilder.append("    Закрыт фискальный режим:   " + stringifyBool(requestResult.fiscalModeClosed) + "\n");
    stringBuilder.append("    Закончена передача данных: " + stringifyBool(requestResult.fiscalDataSendingFinished) + "\n");
    stringBuilder.append(divider + '\n');
    stringBuilder.append(
            QString::fromStdString(
                    "Текущий документ     : " + FRDriver::Converters::fnDocumentToString((int) requestResult.currentDocument) + "\n"
            )
    );
    stringBuilder.append(
                    "Есть данные документа: " + stringifyBool(requestResult.documentData) + "\n"
    );
    stringBuilder.append(
                    "Смена открыта        : " + stringifyBool(requestResult.shiftOpened) + "\n"
    );
    stringBuilder.append(
                    "Флаги предупреждения : " + QString::number(requestResult.warningsFlags, 2) + "\n"
    );

    stringBuilder.append(
                    "Дата и время         : " + dateTime(requestResult.date.day,
                                                         requestResult.date.month,
                                                         requestResult.date.year,
                                                         requestResult.time.hour,
                                                         requestResult.time.minute,
                                                         requestResult.time.second
                    ) + "\n"
    );

    stringBuilder.append(
            QString::fromStdString(
                    "Номер ФН:            : " + requestResult.fnNumber
            ) + "\n"
    );
    stringBuilder.append(
                    "Номер последнего ФД  : " + QString::number(requestResult.lastDocumentNumber) + "\n"
    );

    stringBuilder.append(divider);

    ui()->commandsFNStateLogPlainTextEdit->setPlainText(stringBuilder);
}

CommandsTabController *FNStateCommandsTabController::commandsTabController() const
{
    return (CommandsTabController *) parentController()->parentController();
}

QString FNStateCommandsTabController::stringifyBool(bool val)
{
    return val ? "Да" : "Нет";
}

QString FNStateCommandsTabController::dateTime(uint8_t d,
                                               uint8_t m,
                                               uint8_t y,
                                               uint8_t hour,
                                               uint8_t min,
                                               uint8_t seconds)
{
    return QString::number(d) + '.' + QString::number(m) + ".20" + QString::number(y) + ' ' +
           QString::number(hour) + ':' + QString::number(min) + ':' + QString::number(seconds);
}
