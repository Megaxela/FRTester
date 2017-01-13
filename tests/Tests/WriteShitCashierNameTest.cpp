//
// Created by megaxela on 12.01.17.
//

#include "WriteShitCashierNameTest.h"
#include <Testing/TestLogger.h>
#include <include/Tools/Codecs.h>
#include <include/Tools/Logger.h>

WriteShitCashierNameTest::WriteShitCashierNameTest(TestEnvironment *environment) :
    AbstractTest(
            environment,
            "Запись мусора в имена кассиров",
            "Тест, записывающий все 255 байт в первых 4 кассиров, потом бьет 4 чека и проверяет отправку в ОФД."),
    m_sysAdmPwd(30),
    m_cashierPasswords({1, 2, 3, 4})


{

}

bool WriteShitCashierNameTest::execute()
{
    enviroment()->logger()->log("Запоминаем старые имена кассиров.");

    std::vector<std::string> oldNames;

    for (uint32_t i = 0; i < 4; ++i)
    {
        auto result = enviroment()->driver()->readTableStr(
                m_sysAdmPwd,
                2,
                (uint8_t) (i + 1),
                2
        );

        if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            enviroment()->logger()->log(
                    "Не удалось записать данные. Ошибка #" +
                    std::to_string((int) enviroment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString(
                            (int) enviroment()->driver()->getLastError()
                    )
            );

            return false;
        }

        auto oldNameUtf8 = Codecs::instance()
                .convert("cp1251", "utf-8", QByteArray::fromStdString(result))
                .toStdString();

        oldNames.push_back(oldNameUtf8);

        enviroment()->logger()->log(
                "Старое название кассира №" + std::to_string(i + 1) + oldNameUtf8
        );
    }

    enviroment()->logger()->log("Начинается запись мусора в названия (пропуская 0x00).");

    uint8_t byteValue = 0x01;
    std::string nameString;
    for (uint32_t cashierIndex = 0; cashierIndex < 4; ++cashierIndex)
    {
        nameString.clear();
        for (uint32_t byteIndex = 0; byteIndex < 64; ++byteIndex)
        {
            nameString += byteValue++;
        }

        enviroment()->logger()->log(
                "Запись названия для " +
                std::to_string(cashierIndex) +
                " кассира."
        );

        enviroment()->driver()->writeTable(
                m_sysAdmPwd,
                2,
                (cashierIndex + 1),
                2,
                nameString
        );

        if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            enviroment()->logger()->log(
                    "Не удалось записать данные. Ошибка #" +
                    std::to_string((int) enviroment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString(
                            (int) enviroment()->driver()->getLastError()
                    )
            );

            return true;
        }
    }

    enviroment()->logger()->log("Данные записаны.");

    return false;
}