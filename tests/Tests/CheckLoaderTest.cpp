//
// Created by megaxela on 11.01.17.
//

#include <include/Testing/TestCore.h>
#include <include/Testing/FROperations.h>
#include <thread>
#include "CheckLoaderTest.h"
#include "Testing/TestLogger.h"

CheckLoaderTest::CheckLoaderTest(TestEnvironment *environment) :
    AbstractTest(environment,
                 "Нагрузочный тест с операциями",
                 "Тест бьющий чеки по 500 позиций."),
    m_pwd(30),
    m_numberOfChecks(50),
    m_numberOfOperations(500),
    m_goodPrice(5000),
    m_goodCount(1000)
{

}

bool CheckLoaderTest::execute()
{
    enviroment()->driver()->openShift(m_pwd);

    enviroment()->logger()->log("Начинаем пробивать чеки.");

    for (uint32_t checkIndex = 0;
         checkIndex < m_numberOfChecks;
         ++checkIndex)
    {
        for (uint32_t operationIndex = 0;
             operationIndex < m_numberOfOperations;
             ++operationIndex)
        {
            // Попытка произвести действие
            {
                int tries = 10;
                do
                {
                    bool result = enviroment()->driver()->sell(
                            m_pwd,
                            m_goodCount,
                            m_goodPrice,
                            1,
                            0,
                            0,
                            0,
                            0,
                            "TestGood"
                    );

                    if ((int) enviroment()->driver()->getLastError() != 0 &&
                        (int) enviroment()->driver()->getLastError() != 80)
                    {
                        --tries;
                    }
                }
                while (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError &&
                       tries > 0);

                if (tries == 0)
                {
                    enviroment()->logger()->log(
                            "Не удалось произвести продажу. Последняя ошибка: #" +
                            std::to_string((int) enviroment()->driver()->getLastError()) +
                            ' ' +
                            FRDriver::Converters::errorToString(
                                    (int) enviroment()->driver()->getLastError()
                            )
                    );

                    return false;
                }

                CHECK_TRIGGERS;
            }

            if (operationIndex % (m_numberOfOperations / 5) == 0)
            {
                enviroment()->logger()->log(
                        "Пробито " +
                        std::to_string(operationIndex + 1) +
                        '/' +
                        std::to_string(m_numberOfOperations)
                );
            }
        }

        enviroment()->logger()->log(
                "Пробито " +
                std::to_string(m_numberOfOperations) +
                '/' +
                std::to_string(m_numberOfOperations) +
                ". Закрываем чек."
        );

        {
            int tries = 10;

            do
            {
                enviroment()->driver()->closeCheck(
                        m_pwd,
                        FROperations::smartRound(
                                m_goodPrice * (m_goodCount * 0.001)
                        ) * m_numberOfOperations,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        "Check #" +
                        std::to_string(checkIndex) +
                        '/' +
                        std::to_string(m_numberOfChecks)
                );

                if ((int) enviroment()->driver()->getLastError() != 0 &&
                    (int) enviroment()->driver()->getLastError() != 80)
                {
                    --tries;
                }
            }
            while (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError &&
                   tries > 0);

            if (tries == 0)
            {
                enviroment()->logger()->log(
                        "Ошибка на чеке " +
                        std::to_string(checkIndex) +
                        '/' +
                        std::to_string(m_numberOfChecks)
                );

                enviroment()->logger()->log(
                        "Не удалось закрыть чек в течении 10 попыток. Последняя ошибка: #" +
                        std::to_string((int) enviroment()->driver()->getLastError()) +
                        ' ' +
                        FRDriver::Converters::errorToString(
                                (int) enviroment()->driver()->getLastError()
                        )
                );

                return false;
            }

            CHECK_TRIGGERS;
        }

        enviroment()->logger()->log(
                "Пробит чек " +
                std::to_string(checkIndex + 1) +
                '/' +
                std::to_string(m_numberOfChecks)
        );
    }

    return true;
}
