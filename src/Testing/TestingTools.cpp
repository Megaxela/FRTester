//
// Created by megaxela on 03.02.17.
//

#include <Tools/Time.h>
#include <Testing/TestLogger.h>
#include <Tools/Logger.h>
#include <Testing/TestingTools.h>
#include <Executor/TestingExecutor.h>
#include <QMessageBox>
#include <Windows/Controllers/TestControllers/UnitTestsController.h>
#include <QObject>
#include <Implementation/COMInterface.h>

TestingTools::TestingTools(TestDriver *testDriver, TestLogger* logger) :
    m_executor(nullptr),
    m_currentDriver(testDriver),
    m_testDriver(testDriver),
    m_logger(logger),
    m_parentWidget(nullptr),
    m_controller(nullptr)
{

}

TestingTools::~TestingTools()
{

}

void TestingTools::enableTesting(bool enabled)
{
    if (enabled)
    {
        m_currentDriver = m_testDriver;
    }
    else
    {
        m_currentDriver = m_testDriver->pureDriver();
    }
}

bool TestingTools::waitForPrintingFinished(uint32_t password, uint32_t timeout)
{
    Time::time_t start = Time::get<std::chrono::milliseconds>();

    m_logger->log("Ожидание окончания печати чека.");
    while (true)
    {
        auto state = m_currentDriver->shortStateRequest(password);

        if (m_currentDriver->getLastError() != FRDriver::ErrorCode::NoError)
        {
            m_logger->log(
                    "Не удалось выполнить короткий запрос состояния. Ошибка #" +
                    std::to_string((int) m_currentDriver->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) m_currentDriver->getLastError())
            );
            return false;
        }

        if (state.posSubMode == 0)
        {
            m_logger->log("ФР завершил печать.");
            return true;
        }

        if (Time::get<std::chrono::milliseconds>() - start >= timeout && timeout != 0)
        {
            m_logger->log(
                    "Таймаут ожидания печати. Печать не завершилась за " +
                    std::to_string(timeout) + "мс."
            );

            if (!messageQuestion(
                    "Печать не завершилась за " +
                    std::to_string(timeout) + "мс. Кончилась бумага?",
                    "Да",
                    "Нет"
            ))
            {
                return false;
            }

            start = Time::get<std::chrono::milliseconds>();

            if (!m_currentDriver->resumePrinting(password))
            {
                m_logger->log("Не удалось продолжить печать. " +
                              getErrorString());

                return false;
            }
        }
    }
}

TestingTools::Printing TestingTools::enablePrinting(uint32_t password, TestingTools::Printing mode)
{
    // Считываем прошлое состояние
    auto initialPrintValue = m_currentDriver->readTableBin(
            password,
            17,
            1,
            7
    );

    if (m_currentDriver->getLastError() != FRDriver::ErrorCode::NoError)
    {
        m_logger->log("Не удалось получить текущее значение печати чека. Ошибка: #" +
                      std::to_string((int) m_currentDriver->getLastError()) +
                      ' ' +
                      FRDriver::Converters::errorToString((int) m_currentDriver->getLastError()));
        return Printing::Unknown;
    }

    Printing currentValue;

    switch (initialPrintValue)
    {
    case 0: // Печать включена
        currentValue = Printing::Enabled;
        break;
    case 1: // Печать выключена для одного чека
        currentValue = Printing::DisabledForOneCheck;
        break;
    case 2: // Печать выключена
        currentValue = Printing::Disabled;
        break;
    default:
        currentValue = Printing::Unknown;
        break;
    }

    if (currentValue != mode)
    {
        switch (mode)
        {
        case Printing::Unknown:
            Error("В качестве режима был передан неизвестный режим.");
            return Printing::Unknown;
        case Printing::Enabled:
            m_logger->log("Включаем печать чеков.");
            if (!m_currentDriver->writeTable(
                    password,
                    17,
                    1,
                    7,
                    0, // Значение
                    1  // Размер значения
            ))
            {
                m_logger->log(
                        "Не удалось включить печать чеков. Ошибка: #" +
                        std::to_string((int) m_currentDriver->getLastError()) +
                        ' ' +
                        FRDriver::Converters::errorToString((int) m_currentDriver->getLastError())
                );
                return Printing::Unknown;
            }
            break;
        case Printing::DisabledForOneCheck:
            m_logger->log("Отключаем печать чеков для одного документа.");
            if (!m_currentDriver->writeTable(
                    password,
                    17,
                    1,
                    7,
                    1, // Значение
                    1  // Размер значения
            ))
            {
                m_logger->log("Не удалось отключить печать чеков. Ошибка: #" +
                              std::to_string((int) m_currentDriver->getLastError()) +
                              ' ' +
                              FRDriver::Converters::errorToString((int) m_currentDriver->getLastError()));
                return Printing::Unknown;
            }
            break;
        case Printing::Disabled:
            m_logger->log("Отключаем печать чеков.");
            if (!m_currentDriver->writeTable(
                    password,
                    17,
                    1,
                    7,
                    2, // Значение
                    1  // Размер значения
            ))
            {
                m_logger->log("Не удалось отключить печать чеков. Ошибка: #" +
                              std::to_string((int) m_currentDriver->getLastError()) +
                              ' ' +
                              FRDriver::Converters::errorToString((int) m_currentDriver->getLastError()));
                return Printing::Unknown;
            }
            break;
        }
    }

    return currentValue;
}

bool TestingTools::testingStoped()
{
    return !m_executor->isTestingRunning();
}

bool TestingTools::messageQuestion(const std::string &question,
                                   const std::string &acceptMessage,
                                   const std::string &declineMessage)
{
    QEventLoop loop;

    QObject::connect(m_controller,
                     &UnitTestsController::onMessageBoxClosed,
                     &loop,
                     &QEventLoop::quit);

    emit m_controller->openMessageQuestionSignal(
            QString::fromStdString(question),
            QString::fromStdString(acceptMessage),
            QString::fromStdString(declineMessage)
    );

    loop.exec();

    return m_controller->messageQuestionResult();
}

void TestingTools::messageNotify(const std::string &text)
{
    QEventLoop loop;

    QObject::connect(m_controller,
                     &UnitTestsController::onMessageBoxClosed,
                     &loop,
                     &QEventLoop::quit);

    emit m_controller->openMessageNotifySignal(
            QString::fromStdString(text)
    );

    loop.exec();
}

void TestingTools::setParentWidget(QWidget *parent)
{
    m_parentWidget = parent;
}

void TestingTools::setUnitTestsController(UnitTestsController *controller)
{
    m_controller = controller;
}

bool TestingTools::performConnection(bool askUser)
{
    if (askUser && !messageQuestion(
            "Соединение с ФР было разорвано. Продолжить работу теста?",
            "Да",
            "Нет"
    ))
    {
        return false;
    }

    if (typeid(*m_currentDriver->physicalInterface()) ==
        typeid(COMInterface))
    {
        m_logger->log(
                "Обнаружено соединение по COM порту.\n"
                        "Во избежании включения XMODEM ждем 5 секунд."
        );

        Time::sleep<std::chrono::seconds>(5);
    }
    else
    {
        m_logger->log(
                "Для соединений по TCP можно не ждать отключения режима прошивки по XMODEM."
        );
    }

    m_logger->log("Пытаемся подключиться после отключения.");

    bool connected = false;

    while (!connected)
    {
        int tries = 10;
        while (!m_currentDriver->physicalInterface()->openConnection())
        {
            Time::sleep<std::chrono::milliseconds>(500);
            tries--;
            if (tries == 0)
            {
                if (!messageQuestion(
                        "Не удалось подключиться в течении 10 попыток. Попытаться еще 10 раз?",
                        "Да",
                        "Нет"
                ))
                {
                    return false;
                }

                tries = 10;
            }
        }

        m_logger->log("Удалось подключиться. Проверяем наличие соединения.");

        if (!m_currentDriver->checkConnection())
        {
            m_logger->log("Вне зависимости от того, что подключение установить удалось. Связь с устройством восстановить не удалось. Пытаемся еще раз.");
            continue;
        }

        connected = true;
    }

    return true;
}

bool TestingTools::safeOperation(std::function<void(/*_Args&&...*/)> methodToBeSafe)
{
    bool done = false;

    while (!done)
    {
        methodToBeSafe();

        if (m_currentDriver->getLastError() == FRDriver::ErrorCode::Unknown)
        {
            if (!performConnection())
            {
                m_logger->log("Не удалось совершить действие. Нет соединения с устройством.");
                return false;
            }

            continue;
        }

        done = true;
    }

    return true;
}

std::string TestingTools::getErrorString()
{
    auto err = (int) m_currentDriver->getLastError();

    return "#" +
            std::to_string(err) +
            ' ' +
            FRDriver::Converters::errorToString(err);
}
