//
// Created by megaxela on 17.11.16.
//

#include <include/DriverHolder.h>
#include <Tools/Logger.h>
#include <QtWidgets/QMessageBox>
#include <QtCore/QStringBuilder>
#include "Windows/Controllers/CommandsControllers/StateCommandsTabController.h"

#include "ui_mainwindow.h"

StateCommandsTabController::StateCommandsTabController(Ui::MainWindow *ptr, QWidget* parent) :
        AbstractTabController(ptr, parent, nullptr)
{

}

StateCommandsTabController::~StateCommandsTabController()
{

}

void StateCommandsTabController::setupConnections()
{
    // Кнопка запроса короткого состояния
    connect(ui()->commandsShortStateRequestPushButton,
            &QPushButton::clicked,
            this,
            &StateCommandsTabController::onShortRequest);

    // Кнопка запроса полного состояния
    connect(ui()->commandsFullStateRequestPushButton,
            &QPushButton::clicked,
            this,
            &StateCommandsTabController::onFullRequest);

    // Кнопка запроса необнуляемых сумм
    connect(ui()->commandsNonZeroSumsRequestPushButton,
            &QPushButton::clicked,
            this,
            &StateCommandsTabController::onZeroSumsRequest);

    // Кнопка запроса подытога чека
    connect(ui()->commandsCheckResultRequestPushButton,
            &QPushButton::clicked,
            this,
            &StateCommandsTabController::onCheckResultRequest);
}

void StateCommandsTabController::configureWidgets()
{
    int id = QFontDatabase::addApplicationFont(":/files/fonts/FiraCode-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont font(family);

    ui()->commandsStatePlainTextEdit->setFont(font);
}

void StateCommandsTabController::onFullRequest()
{
    Log("Выполняем полный запрос состояния.");
    ExcessLog("Проверка наличия соединения");
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ExcessLog("Запрос на ФР.");
    auto requestResult = DriverHolder::driver().fullStateRequest(
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

    stringBuilder.append(divider + "\n");
    stringBuilder.append("Полный запрос состояния:\n");
    stringBuilder.append(divider + "\n");

    stringBuilder.append("Версия прошивки: " +
                         QString(requestResult.firmwareVersion.majorVersion) +
                         "." +
                         QString(requestResult.firmwareVersion.minorVersion) + '\n');
    stringBuilder.append("Версия сборки:   " +
                         QString::number(requestResult.firmwareBuild) + '\n');
    stringBuilder.append("Дата сборки:     " +
                         stateDateToString(requestResult.firmwareDate) + '\n');
    stringBuilder.append(divider + "\n");

    stringBuilder.append("Текущий режим             : " +
                         QString::number(requestResult.posMode) +
                         " - " +
                         QString::fromStdString(FRDriver::Converters::posModeToString(requestResult.posMode)) + "\n");
    stringBuilder.append("Текущий подрежим          : " + QString::number(requestResult.posSubMode) +
                         " - " +
                         QString::fromStdString(FRDriver::Converters::posSubModeToString(requestResult.posSubMode)) + "\n");
    stringBuilder.append("Номер ККТ в зале: " + QString::number(requestResult.numberInHall) + '\n');
    stringBuilder.append("Номер документа:  " + QString::number(requestResult.currentDocumentPassthrougNumber) + '\n');
    stringBuilder.append("Номер порта:      " + QString::number(requestResult.posPort) + '\n');
    stringBuilder.append("Последняя закрытая смена: " + QString::number(requestResult.lastClosedShiftNumber) + '\n');
    stringBuilder.append("Дата:  " + stateDateToString(requestResult.date) + '\n');
    stringBuilder.append("Время: " + stateTimeToString(requestResult.time) + '\n');
//    stringBuilder.append("Серийный номер: " + QString::number(requestResult.factoryNumberUpper) + '\n')
    stringBuilder.append(divider + "\n");
    stringBuilder.append("Флаги ФР: 0b" + QString::number(requestResult.posFlags, 2) + "\n");

    stringBuilder.append(divider + "\n");

    stringBuilder.append("0.   Рулон операционного журнала             : " + QString((1 << 0 ) & requestResult.posFlags ? "есть" : "нет") + "\n");
    stringBuilder.append("1.   Рулон чековой ленты                     : " + QString((1 << 1 ) & requestResult.posFlags ? "есть" : "нет") + "\n");
    stringBuilder.append("2.   Верхний датчик подкладного документа    : " + QString((1 << 2 ) & requestResult.posFlags ? "да" : "нет") + "\n");
    stringBuilder.append("3.   Нижний датчик подкладного документа     : " + QString((1 << 3 ) & requestResult.posFlags ? "да" : "нет") + "\n");
    stringBuilder.append("4.   Положение десятичной точки              : " + QString((1 << 4 ) & requestResult.posFlags ? "2 знака" : "0 знаков") + "\n");
    stringBuilder.append("6.   Оптический датчик операционного журнала : " + QString((1 << 6 ) & requestResult.posFlags ? "бумага есть" : "бумаги нет") + "\n");
    stringBuilder.append("7.   Оптический датчик чековой ленты         : " + QString((1 << 7 ) & requestResult.posFlags ? "бумага есть" : "бумаги нет") + "\n");
    stringBuilder.append("8.   Рычаг термоголовки операционного журнала: " + QString((1 << 8 ) & requestResult.posFlags ? "поднят" : "опущен") + "\n");
    stringBuilder.append("9.   Рычаг термоголовки чековой ленты        : " + QString((1 << 9 ) & requestResult.posFlags ? "поднят" : "опущен") + "\n");
    stringBuilder.append("10.  Крышка корпуса ККТ                      : " + QString((1 << 10) & requestResult.posFlags ? "поднята" : "опущена") + "\n");
    stringBuilder.append("11.  Денежный ящик                           : " + QString((1 << 11) & requestResult.posFlags ? "открыт" : "закрыт") + "\n");
    stringBuilder.append("12а. Отказ правого датчика принтера          : " + QString((1 << 12) & requestResult.posFlags ? "да" : "нет") + "\n");
    stringBuilder.append("12б. Бумага на входе в презентер             : " + QString((1 << 12) & requestResult.posFlags ? "да" : "нет") + "\n");
    stringBuilder.append("12в. Модель принтера                         : " + QString((1 << 12) & requestResult.posFlags ? "MLT-286-1" : "MLT-286") + "\n");
    stringBuilder.append("12г. Крышка корпуса ККТ операционного журнала: " + QString((1 << 12) & requestResult.posFlags ? "поднята" : "опущена") + "\n");
    stringBuilder.append("13а. Отказ левого датчика принтера           : " + QString((1 << 13) & requestResult.posFlags ? "да" : "нет") + "\n");
    stringBuilder.append("13б. Бумага на выходе из презентера          : " + QString((1 << 13) & requestResult.posFlags ? "да" : "нет") + "\n");
    stringBuilder.append("15а. Увеличенная точность количества         : " + QString((1 << 15) & requestResult.posFlags ? "увеличенная точность" : "нормальная точность") + "\n");
    stringBuilder.append("15б. Буфер принтера непуст                   : " + QString((1 << 15) & requestResult.posFlags ? "непуст" : "пуст") + "\n");

    stringBuilder.append(divider + "\n");

    ui()->commandsStatePlainTextEdit->setPlainText(stringBuilder);
}

void StateCommandsTabController::onShortRequest()
{
    Log("Выполняем короткий запрос состояния");
    ExcessLog("Проверка наличия соединения");
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ExcessLog("Запрос на ФР.");
    auto requestResult = DriverHolder::driver().shortStateRequest(
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

    stringBuilder += divider + "\n";

    stringBuilder += "Короткий запрос состояния\n";

    stringBuilder += divider + "\n";
    stringBuilder += "Текущий режим             : " +
                     QString::number(requestResult.posMode) +
                     " - " +
                     QString::fromStdString(FRDriver::Converters::posModeToString(requestResult.posMode)) + "\n";

    stringBuilder += "Текущий подрежим          : " + QString::number(requestResult.posSubMode) +
                     " - " +
                     QString::fromStdString(FRDriver::Converters::posSubModeToString(requestResult.posSubMode)) + "\n";

    stringBuilder += "Результат последней печати: " +
                     QString::fromStdString(FRDriver::Converters::lastPrintResultToString(requestResult.lastPrintResult)) + "\n";

    stringBuilder += divider + "\n";

    stringBuilder += "Флаги ФР: 0b" + QString::number(requestResult.posFlags, 2) + "\n";

    stringBuilder += divider + "\n";

    stringBuilder += "0.   Рулон операционного журнала             : " + QString((1 << 0 ) & requestResult.posFlags ? "есть" : "нет") + "\n";
    stringBuilder += "1.   Рулон чековой ленты                     : " + QString((1 << 1 ) & requestResult.posFlags ? "есть" : "нет") + "\n";
    stringBuilder += "2.   Верхний датчик подкладного документа    : " + QString((1 << 2 ) & requestResult.posFlags ? "да" : "нет") + "\n";
    stringBuilder += "3.   Нижний датчик подкладного документа     : " + QString((1 << 3 ) & requestResult.posFlags ? "да" : "нет") + "\n";
    stringBuilder += "4.   Положение десятичной точки              : " + QString((1 << 4 ) & requestResult.posFlags ? "2 знака" : "0 знаков") + "\n";
    stringBuilder += "6.   Оптический датчик операционного журнала : " + QString((1 << 6 ) & requestResult.posFlags ? "бумага есть" : "бумаги нет") + "\n";
    stringBuilder += "7.   Оптический датчик чековой ленты         : " + QString((1 << 7 ) & requestResult.posFlags ? "бумага есть" : "бумаги нет") + "\n";
    stringBuilder += "8.   Рычаг термоголовки операционного журнала: " + QString((1 << 8 ) & requestResult.posFlags ? "поднят" : "опущен") + "\n";
    stringBuilder += "9.   Рычаг термоголовки чековой ленты        : " + QString((1 << 9 ) & requestResult.posFlags ? "поднят" : "опущен") + "\n";
    stringBuilder += "10.  Крышка корпуса ККТ                      : " + QString((1 << 10) & requestResult.posFlags ? "поднята" : "опущена") + "\n";
    stringBuilder += "11.  Денежный ящик                           : " + QString((1 << 11) & requestResult.posFlags ? "открыт" : "закрыт") + "\n";
    stringBuilder += "12а. Отказ правого датчика принтера          : " + QString((1 << 12) & requestResult.posFlags ? "да" : "нет") + "\n";
    stringBuilder += "12б. Бумага на входе в презентер             : " + QString((1 << 12) & requestResult.posFlags ? "да" : "нет") + "\n";
    stringBuilder += "12в. Модель принтера                         : " + QString((1 << 12) & requestResult.posFlags ? "MLT-286-1" : "MLT-286") + "\n";
    stringBuilder += "12г. Крышка корпуса ККТ операционного журнала: " + QString((1 << 12) & requestResult.posFlags ? "поднята" : "опущена") + "\n";
    stringBuilder += "13а. Отказ левого датчика принтера           : " + QString((1 << 13) & requestResult.posFlags ? "да" : "нет") + "\n";
    stringBuilder += "13б. Бумага на выходе из презентера          : " + QString((1 << 13) & requestResult.posFlags ? "да" : "нет") + "\n";
    stringBuilder += "15а. Увеличенная точность количества         : " + QString((1 << 15) & requestResult.posFlags ? "увеличенная точность" : "нормальная точность") + "\n";
    stringBuilder += "15б. Буфер принтера непуст                   : " + QString((1 << 15) & requestResult.posFlags ? "непуст" : "пуст") + "\n";

    stringBuilder += divider + "\n";

    ui()->commandsStatePlainTextEdit->setPlainText(stringBuilder);
}

CommandsTabController *StateCommandsTabController::commandsTabController() const
{
    return (CommandsTabController *) parentController();
}

void StateCommandsTabController::onZeroSumsRequest()
{
    Log("Выполняем запрос ненулевых сумм");
    ExcessLog("Проверка наличия соединения");
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ExcessLog("Запрос на ФР.");
    auto requestResult = DriverHolder::driver().getNonZeroSums();

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
    stringBuilder.append("Необнуляемые суммы\n");
    stringBuilder.append(divider + '\n');

    stringBuilder.append("Сумма №" +
                         QString::number(1) +
                         ": " +
                         QString::number(requestResult.incomingSum / 100.0, 'f') +
                         '\n');

    stringBuilder.append("Сумма №" +
                         QString::number(2) +
                         ": " +
                         QString::number(requestResult.consumptionSum / 100.0, 'f') +
                         '\n');

    stringBuilder.append("Сумма №" +
                         QString::number(3) +
                         ": " +
                         QString::number(requestResult.returnIncomingSum / 100.0, 'f') +
                         '\n');

    stringBuilder.append("Сумма №" +
                         QString::number(4) +
                         ": " +
                         QString::number(requestResult.returnConsumptionSum / 100.0, 'f') +
                         '\n');

    stringBuilder.append(divider + '\n');

    ui()->commandsStatePlainTextEdit->setPlainText(stringBuilder);
}

void StateCommandsTabController::onCheckResultRequest()
{
    Log("Выполняем запрос подытога чека.");

    ExcessLog("Проверка наличия соединения");
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ExcessLog("Запрашиваем подытог.");

    auto result = DriverHolder::driver().checkResult(
            commandsTabController()->password()
    );

    commandsTabController()->setLastStatus();

    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        return;
    }

    QString textResult = "Подытог чека: " +
            QString::number(result * 0.01, 'f', 2);

    ui()->commandsStatePlainTextEdit->setPlainText(textResult);
}

QString StateCommandsTabController::stateDateToString(const FRDriver::DateStructure &date)
{
    QString string;

    string.append(QString::number(date.day));
    string.append('.');
    string.append(QString::number(date.month));
    string.append('.');
    string.append(QString::number(date.year));

    return string;
}

QString StateCommandsTabController::stateTimeToString(const FRDriver::TimeStructure &time)
{
    QString string;

    string.append(QString::number(time.hour));
    string.append(':');
    string.append(QString::number(time.minute));
    string.append(':');
    string.append(QString::number(time.second));

    return string;
}
