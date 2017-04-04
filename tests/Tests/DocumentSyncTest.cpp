//
// Created by megaxela on 30.03.17.
//

#include <Testing/StaticTestFabric.h>
#include <shared_libs/frdrvcross/include/Tools/Time.h>
#include <shared_libs/frdrvcross/include/Implementation/COMInterface.h>
#include <include/Tools/Codecs.h>
#include <include/Testing/FROperations.h>
#include <shared_libs/frdrvcross/include/Tools/Logger.h>
#include "DocumentSyncTest.h"

REGISTER_STATIC_TEST(DocumentSyncTest)

DocumentSyncTest::DocumentSyncTest() :
    AbstractTest(nullptr,
                 "Стабильный тест для проверки синхронизации документов",
                 "",
                 {{"Password", (uint32_t) 30},
                  {"Checks in change", (uint32_t) 20},
                  {"Number of changes", (uint32_t) 1},
                  {"Number of operations", (uint32_t) 100}}),
    m_randomDevice(),
    m_rng()

{

}

bool DocumentSyncTest::execute()
{
    uint32_t password = getValue("Password").toUInt32();
    uint32_t numberOfChecks = getValue("Checks in change").toUInt32();
    uint32_t numberOfShifts = getValue("Number of changes").toUInt32();
    uint32_t numberOfOperations = getValue("Number of operations").toUInt32();
    uint32_t numberOfPrints = 10;

    uint32_t operationsPrintDelta = numberOfOperations / numberOfPrints;


    for (uint32_t shiftIndex = 0;
         shiftIndex < numberOfShifts;
         ++shiftIndex)
    {
        CHECK_IS_TEST_RUNNING

        uint64_t shiftOperationMoney = 0;

        environment()->logger()->log(
                "Открываем " +
                std::to_string(shiftIndex + 1) +
                " смену из " +
                std::to_string(numberOfShifts)
        );

        if (!environment()->tools()->safeOperation(
                [=]()
                {
                    environment()->driver()->openShift(password);
                }
        ))
        {
            return false;
        }

        // Ожидаем окончания печати
        if (!environment()->tools()->waitForPrintingFinished(password, 10000))
        {
            environment()->logger()->log("Не удалось дождаться окончания печати.");
            return false;
        }

        environment()->logger()->log("Дождались окончания печати документа открытия смены.");



        FRDriver::FNStatus fnStatus;
        if (!environment()->tools()->safeOperation(
                [&]()
                {
                    fnStatus = environment()->driver()->getFNStatus(password);
                }
        ))
        {
            return false;
        }

        // Проверяем на наличие рядовых ошибок
        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Во время получения статуса FN была получена ошибка. #" +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    " " +
                    FRDriver::Converters::errorToString(
                            (int) environment()->driver()->getLastError()
                    )
            );

            return false;
        }

        // Проверяем состояние в ККТ
        FRDriver::ShortState shortState;
        if (!environment()->tools()->safeOperation(
                [&]()
                {
                    shortState = environment()->driver()->shortStateRequest(password);
                }
        ))
        {
            return false;
        }

        // Проверяем на наличие рядовых ошибок
        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Во время получения короткого запроса состояния была получена ошибка. #" +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    " " +
                    FRDriver::Converters::errorToString(
                            (int) environment()->driver()->getLastError()
                    )
            );

            return false;
        }

        if (shortState.posMode != 2)
        {
            environment()->logger()->log("Смена не открылась.");
            if (!environment()->tools()->messageQuestion(
                    "Смена не открылась, что за дела?",
                    "Да нормально все.",
                    "Чет не так."
            ))
            {
                environment()->logger()->log("Ну тогда останавливаем тест.");
                return false;
            }
        }

        if (!fnStatus.shiftOpened)
        {
            environment()->logger()->log("Смена в ФН не открылась. Хотя в ККТ открылась.");
            if (!environment()->tools()->messageQuestion(
                    "Смена в ФН не открылась, что за дела?",
                    "Да нормально все.",
                    "Чет не так."
            ))
            {
                environment()->logger()->log("Ну тогда останавливаем тест.");
                return false;
            }
        }

        for (uint32_t checkIndex = 0;
             checkIndex < numberOfChecks;
             ++checkIndex)
        {
            CHECK_IS_TEST_RUNNING

            uint64_t checkOperationsMoney = 0;

            std::uniform_int_distribution<uint64_t> generator(1, 50000);

            for (uint32_t operationIndex = 0;
                 operationIndex < numberOfOperations;
                 ++operationIndex)
            {
                CHECK_IS_TEST_RUNNING

                if (operationsPrintDelta == 0 ||
                    ((operationIndex + 1) % operationsPrintDelta) == 0 ||
                    operationIndex == (numberOfOperations - 1))
                {
                    environment()->logger()->log(
                            "Пробивается операция №" +
                            std::to_string(operationIndex + 1) +
                            " из " +
                            std::to_string(numberOfOperations)
                    );
                }

//                uint64_t count = 1000;
//                uint64_t price = 1000;

                uint64_t count = generator(m_rng);
                uint64_t price = generator(m_rng);

                if (!environment()->tools()->safeOperation(
                        [=]()
                        {
                            environment()->driver()->sell(
                                    password,
                                    count,
                                    price,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    Codecs::instance().convert(
                                            "utf-8",
                                            "cp1251",
                                            QByteArray::fromStdString(
                                                    "Операция №" +
                                                    std::to_string(operationIndex) +
                                                    " из " +
                                                    std::to_string(numberOfOperations)
                                            )
                                    ).toStdString()
                            );
                        }
                ))
                {
                    return false;
                }

                // Проверяем открылся ли документ в ФН
                if (!environment()->tools()->safeOperation(
                        [&]()
                        {
                            fnStatus = environment()->driver()->getFNStatus(password);
                        }
                ))
                {
                    return false;
                }

                // Проверяем на наличие рядовых ошибок
                if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
                {
                    environment()->logger()->log(
                            "Во время получения статуса FN была получена ошибка. #" +
                            std::to_string((int) environment()->driver()->getLastError()) +
                            " " +
                            FRDriver::Converters::errorToString(
                                    (int) environment()->driver()->getLastError()
                            )
                    );

                    return false;
                }

                if (fnStatus.currentDocument != FRDriver::FNStatus::Document::Check)
                {
                    environment()->logger()->log(
                            "Чек открыли, но ФН так не думает. Документ в ФН: " +
                            FRDriver::Converters::fnDocumentToString((uint8_t) fnStatus.currentDocument)
                    );

                    if (!environment()->tools()->messageQuestion(
                            "В ФН не открылся документ. Что за дела?",
                            "Да нормально все.",
                            "Чет не так."
                    ))
                    {
                        environment()->logger()->log("Ну тогда останавливаем тест.");
                        return false;
                    }
                }

                // Подсчитываем сумму
                auto result = FROperations::multiply(price, count);

                Log("Чек с ценой " + std::to_string(price) + " и количеством " + std::to_string(count) + " = " + std::to_string(result));

                checkOperationsMoney += result;
            }

            environment()->logger()->log(
                    "Проверяем подытог..."
            );

            // Проверяем подытог
            uint64_t calculatedSum = 0;
            if (!environment()->tools()->safeOperation(
                    [&]()
                    {
                        calculatedSum = environment()
                                ->driver()
                                ->checkResult(password);
                    }
            ))
            {
                return false;
            }

            if (calculatedSum != checkOperationsMoney)
            {
                environment()->logger()->log(
                        "Расхождения подытога и подсчитанной суммы."
                );

                environment()->logger()->log(
                        "Подытог: " + std::to_string(calculatedSum)
                );

                environment()->logger()->log(
                        "Подсчитанная сумма: " + std::to_string(checkOperationsMoney)
                );

                if (!environment()->tools()->messageQuestion(
                        "Было обнаружено расхождение в подытоге. Продолжить тест?",
                        "Да",
                        "Нет"
                ))
                {
                    return false;
                }
            }

            environment()->logger()->log(
                    "Чек пробит. Закрываем его..."
            );

            // Окей, все пробили. Закрываем чек
            uint64_t shortChange = 0;

            // Добавляем случайную сумму для сдачи
            uint64_t randomAdd = generator(m_rng);

            environment()->logger()->log("Случайная добавка: " + std::to_string(randomAdd));

            if (!environment()->tools()->safeOperation(
                    [&]()
                    {
                        auto result = environment()
                                ->driver()
                                ->closeCheck(
                                        password,
                                        checkOperationsMoney + randomAdd,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        Codecs::instance().convert(
                                                "utf-8",
                                                "cp1251",
                                                QByteArray::fromStdString(
                                                        "Чек №" +
                                                        std::to_string(checkIndex + 1) +
                                                        "/" +
                                                        std::to_string(numberOfChecks)
                                                )
                                        ).toStdString()
                        );

                        shortChange = result.change;
                    }
            ))
            {
                return false;
            }

            environment()->logger()->log("Ожидаем окончания печати.");

            // Ожидаем окончания печати
            if (!environment()->tools()->waitForPrintingFinished(password, 10000))
            {
                environment()->logger()->log("Не удалось дождаться окончания печати.");
                return false;
            }

            if (shortChange != randomAdd)
            {
                environment()->logger()->log(
                        "Сдача отличается от посчитанной."
                );

                environment()->logger()->log(
                        "Посчитанная сумма чека: " +
                        std::to_string(checkOperationsMoney)
                );

                environment()->logger()->log(
                        "Сгенерированное добавление к сумме оплаты: " +
                        std::to_string(randomAdd)
                );

                environment()->logger()->log(
                        "Полученная сдача: " +
                        std::to_string(shortChange)
                );

                if (!environment()->tools()->messageQuestion(
                        "Было обнаружено расхождение в сдаче. Продолжить тест?",
                        "Да",
                        "Нет"
                ))
                {
                    return false;
                }
            }

            shiftOperationMoney += checkOperationsMoney;
        }

        environment()->logger()->log(
                "Чеки закончились. Закрываем смену."
        );

        if (!environment()->tools()->safeOperation(
                [=]()
                {
                    environment()->driver()->shiftCloseReport(password);
                }
        ))
        {
            return false;
        }

        environment()->logger()->log("Ожидаем окончания печати.");

        // Ожидаем окончания печати
        if (!environment()->tools()->waitForPrintingFinished(password, 10000))
        {
            environment()->logger()->log("Не удалось дождаться окончания печати.");
            return false;
        }
    }

    return true;
}