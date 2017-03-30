#include <iostream>
#include <memory>
#include <stdlib.h>
#include <unistd.h>
#include "Implementation/COMInterface.h"
#include "FRDriver.h"
#include "Implementation/DefaultProtocol.h"
#include "Tools/Logger.h"
#include "Implementation/TestProtocol.h"
int main(int argc, char** argv)
{

#ifdef OS_WINDOWS
//    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

//    ByteArray arr = ByteArray::fromHex("010203");
//
//    std::cout << (int) arr.read<uint8_t>(0) << std::endl;
//    std::cout << (int) arr.read<uint8_t>(1) << std::endl;
//    std::cout << (int) arr.read<uint8_t>(2) << std::endl;

    if (argc != 3)
    {
        std::cout << "Неверное использование frdrvcross." << std::endl
                  << "    frdrvcross <устройство> <время отправки одного байта в микросекундах>" << std::endl << std::endl
                  << "Пример:" << std::endl
                  << "    frdrvcross COM2 20000" << std::endl
                  << "    frdrvcross /dev/ttyS2 20000" << std::endl;
        return -2;
    }

    Logger::i().setLogLevel(Logger::ErrorClass::Warning);

    auto physicalInterface = std::make_shared<COMInterface>();

    physicalInterface->setDevice(argv[1]);
    physicalInterface->setBaudRate(115200);

    try
    {
        physicalInterface->setByteSendTime(std::stoi(argv[2]));
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Неверное значение числа." << std::endl;
    }
    catch (std::out_of_range &e)
    {
        std::cout << "Слишком большое значение времени." << std::endl;
    }

    auto protocol = std::make_shared<DefaultProtocol>();

    FRDriver driver;

    driver.setInterface(physicalInterface);
    driver.setProtocol(protocol);

    if (!driver.physicalInterface()->openConnection())
    {
        return -1;
    }

    Log("Соединение установлено.");

    if (!driver.resumePrinting(30))
    {
        Error("Не удалось продолжить печать. Вероятно она была и не нужна.");
    }
    else
    {
        Log("С печатью проблем быть не должно.");
    }

    auto result = driver.shortStateRequest(30);

    if (driver.getLastError() != FRDriver::ErrorCode::NoError)
    {
        Error("Ошибочка: " +
              FRDriver::Converters::errorToString(driver.getLastError()));
    }

    FRDriver::ShortState state = driver.shortStateRequest(30);

    if (driver.getLastError() != FRDriver::ErrorCode::NoError)
    {
        Error("Не удалось получить состояние ФР. Ошибка: " +
              FRDriver::Converters::errorToString(driver.getLastError()));
        return -2;
    }

    Log("Текущее состояние ФР: " + FRDriver::Converters::posModeToString(state.posMode));

    if (state.posSubMode != 0)
    {
        Log("Подрежим ККТ сообщает, что у него какие-то проблемы с бумагой. (" +
            std::to_string(state.posSubMode) +
            ") пробуем продолжить печать.");

        if (!driver.resumePrinting(30))
        {
            Error("Не удалось продолжить печать. Вероятно ошибка в чем-то другом. Ошибка при продолжнии: " +
                  FRDriver::Converters::errorToString(driver.getLastError()));
            return -3;
        }
        Log("Печать продолжена.");

        Time::sleep<std::chrono::milliseconds>(100);
    }

    if (state.posMode == 3)
    {
        Log("Кончились 24 часа. Закрываем смену.");
        Log("Закрываем открытую смену");
        if (!driver.shiftCloseReport(30))
        {
            Error("Не удается закрыть смену. Ошибка: " +
                          FRDriver::Converters::errorToString(driver.getLastError()));
            return -1;
        }

        Log("Теперь открываем смену");
        if (!driver.openShift(30))
        {
            Error("Не удается открыть смену. Ошибка: " +
                  FRDriver::Converters::errorToString(driver.getLastError()));
            return -1;
        }
    }
    else if (state.posMode == 4)
    {
        Log("Открываем смену.");
        if (!driver.openShift(30))
        {
            Error("Не удается открыть смену. Ошибка: " +
                  FRDriver::Converters::errorToString(driver.getLastError()));
            return -1;
        }
    }
    else
    {
        if (state.posMode == 8)
        {
            Log("Открыт незакрытый чек. Отменяем его.");

            auto checkResult = driver.cancelCheck(
                    30 // Пароль
            );

            if (driver.getLastError() != FRDriver::ErrorCode::NoError)
            {
                Error("Не удалось закрыть чек. Ошибка: " +
                      FRDriver::Converters::errorToString(driver.getLastError()));
                return -1;
            }
        }

        Log("Смена открыта. И нет открытых чеков.")
    }

    while (true)
    {
        Log("Открываем чек с 1 продажей.");

        bool s = false;

        for (int i = 0; i < 20; ++i)
        {
            while (!s)
            {
                Log("Пробуем совершить продажу.");
                if (!driver.sell(
                        30, // Пароль
                        100, // Грамы (количество)
                        500, // Стоимость за ед
                        0,  // Отдел
                        0,  // Налог 1
                        0, // Налог 2
                        0, // Налог 3
                        0, // Налог 4
                        "Test good #" + std::to_string(rand())
                ))
                {
                    Error("Не удалось совершить продажу. Ошибка: " +
                          FRDriver::Converters::errorToString(driver.getLastError()));

                    if ((int) driver.getLastError() != 0x50)
                    {
                        return -1;
                    }
                }
                else
                {
                    s = true;
                }
            }
        }

        Log("Теперь в течении 10 секунд запрашиваем короткое состояние.");

        Time::time_t beginTime = Time::getMilliseconds();
        while (Time::getMilliseconds() - beginTime < 10 * 1000)
        {
            auto waitFor = (uint32_t) (1000.0 / 3.0) * 1000;
            for (int i = 0; i < 3; ++i)
            {
                Time::time_t reqBegin = Time::getMicroseconds();
                auto shortResponse = driver.shortStateRequest(30);

                if (driver.getLastError() != FRDriver::ErrorCode::NoError)
                {
                    Error("Возникла ошибка во время короткого запроса состояния. Ошибка: " +
                          FRDriver::Converters::errorToString(driver.getLastError()));
                    return -1;
                }

                Log("Короткий запрос получен:");
                Log("    Текущий режим: " +
                    FRDriver::Converters::posModeToString(shortResponse.posMode));
                Log("    Текущий подрежим: " +
                    std::to_string(shortResponse.posSubMode));

                Time::time_t diff = Time::getMicroseconds() - reqBegin;

                if (diff < waitFor)
                {
//                    Time::sleep<std::chrono::microseconds>(
//                            waitFor - diff
//                    );
                }
                else
                {
//                    Warning("Короткий запрос обрабатывается так долго, что не получится делать 3 запроса в секунду. Ожидалось " +
//                                    std::to_string(waitFor) +
//                                    ", а оказалось " +
//                                    std::to_string(diff));
                }
            }

            // Чистим вилкой
            physicalInterface->write(ByteArray::fromHex("0506"));
        }

//        protocol->setSpendRead(false);

        Log("Закончили опрашивать ФР. Закрываем чек.");
        auto checkResult = driver.closeCheck(
                30, // Пароль
                UINT64_C(2000), // Сумма наличными
                0, // Сумма безналом
                0, // Сумма безналом
                0, // Сумма безналом
                0, // Скидка
                0, // Налог 1
                0, // Налог 2
                0, // Налог 3
                0, // Налог 4
                "Test check"
        );

        if (driver.getLastError() != FRDriver::ErrorCode::NoError)
        {
            Error("Не удалось закрыть чек. Ошибка: " +
                  FRDriver::Converters::errorToString(driver.getLastError()));
            return -1;
        }

        // Ждем пока он напечатает чек
//        Time::sleep<std::chrono::milliseconds>(100);
    }

    return 0;
}