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
    Log("Ан нет, сорян. Еще не реализовал.");
}

void StateCommandsTabController::onShortRequest()
{
    Log("Выполняем короткий запрос состояния");
    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отстуствует.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Соединение с ФР отсутствует."
        );
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
    QString divider = "=================================";

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

    stringBuilder += "Результат последнец печати: " +
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
