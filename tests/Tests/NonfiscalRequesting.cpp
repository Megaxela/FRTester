//
// Created by megaxela on 26.01.17.
//

#include "NonfiscalRequesting.h"
#include "Testing/TestLogger.h"
#include <thread>
#include <chrono>
#include <include/Testing/StaticTestFabric.h>
#include <include/Tools/Codecs.h>

REGISTER_STATIC_TEST(NonfiscalRequesting)

NonfiscalRequesting::NonfiscalRequesting() :
    NonfiscalRequesting(nullptr)
{

}

NonfiscalRequesting::NonfiscalRequesting(TestEnvironment* environment) :
    AbstractTest(environment,
                 "Особый тест для питерского Linux.",
                 "Тест, который выполняет: "
                 "Отрытие нефискального документ, "
                 "Печать 20 нумерованных строк, "
                 "Закрытие нефискального документа, "
                 "Отрезка чека, "
                 "20 коротких запросов.",
                 {{"Password", (uint32_t) 30},
                  {"Number Of Documents", (uint32_t) 150},
                  {"Print QR Code", true},
                  {"Font print", false}})

{

}

bool NonfiscalRequesting::execute()
{
    auto password = getValueUInt32("Password");
    auto docs = getValueUInt32("Number Of Documents");

    for (uint32_t cycle = 0; cycle < docs; ++cycle)
    {

        CHECK_IS_TEST_RUNNING;

        environment()->logger()->log("Открываем нефискальный документ #" + std::to_string(cycle + 1));

        environment()->driver()->openNonFiscalDocument(password);
        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation)
        {
            environment()->logger()->log("    Каким-то образом прошло открытие нефискального документа.");
            return false;
        }

        environment()->logger()->log("    Печать заголовка.");

        environment()->driver()->documentHeaderPrint(
                password,
                Codecs::instance().convert(
                        "utf-8",
                        "cp1251",
                        QByteArray("Текст для печати в заголовке")
                ).toStdString(),
                22
        );

        environment()->logger()->log("    Ожидаем окончания печати чека.");

        if (!environment()->tools()->waitForPrintingFinished(
                password,
                10000
        ))
        {
            environment()->logger()->log("Не удалось дождаться окончания печати чека.");
            return false;
        }

        environment()->logger()->log("    Печать строки для печати.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("Строка для печати")
        ).toStdString(), 1);

        environment()->logger()->log("    Печать количечества на цену.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("                             1.000 \x0B""100.00")
        ).toStdString(), 1);

        environment()->logger()->log("    Печать результирующей суммы.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("                                   \x1F""100.00")
        ).toStdString(), 1);

        environment()->logger()->log("    Печать отдела");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("Отдел №1")
        ).toStdString(), 1);

        environment()->logger()->log("    Печать строки для печати при закрытии чека.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("Закрытый чек")
        ).toStdString(), 1);

        environment()->logger()->log("    Печать итога.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("ИТОГ          \x1F""100.00")
        ).toStdString(), 2);

        environment()->logger()->log("    Печать оплаты.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray(" НАЛИЧНЫМИ                         \x1F""100.00")
        ).toStdString(), 1);

        environment()->logger()->log("    Печать системы налогообложения, ФД и ФП.");

        print(Codecs::instance().convert(
                "utf-8",
                "cp1251",
                QByteArray("СНО:ОСН               ФД:685 ФП:2375300671")
        ).toStdString(), 1);

        environment()->logger()->log("    Ожидаем окончания печати чека.");

        if (!environment()->tools()->waitForPrintingFinished(
                password,
                10000
        ))
        {
            environment()->logger()->log("Не удалось дождаться окончания печати чека.");
            return false;
        }

        environment()->logger()->log("    Переходим к печати QR кода.");
        if (getValue("Print QR Code").toBoolean())
        {
            environment()->logger()->log("    Отправляем данные в ККТ для печати Qr кода.");

            if (!environment()->driver()->loadData(
                    password,
                    0,
                    0,
                    ByteArray(
                            (const uint8_t*) "t=20170313T16&s=0.00&fn=9999078900005580&i=694&fp=4116102537&n=1",
                            64
                    )
            ))
            {
                environment()->logger()->log("        Не удалось загрузить данные в ККТ.");
                return false;
            }

            auto printResult = environment()->driver()->printMultidimensionalBarcode(
                    password,
                    FRDriver::BarcodeType::QRCode1,
                    64,
                    0,
                    0,
                    0,
                    6,
                    0,
                    2,
                    1
            );

            if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
            {
                environment()->logger()->log("        Не удалось напечатать отправленные данные.");
                return false;
            }

            environment()->logger()->log(
                    "        Результирующие размеры печати: " +
                    std::to_string(printResult.barcodeWidth) +
                    " x " +
                    std::to_string(printResult.barcodeHeight)
            );
        }
        else
        {
            environment()->logger()->log("        Печать QR кода отключена.");
        }
        environment()->logger()->log("    Печать QR кода завершена.");


        environment()->logger()->log("Закрываем нефискальный документ #" + std::to_string(cycle + 1));
        environment()->driver()->closeNonFiscalDocument(password);
        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation)
        {
            environment()->logger()->log("Каким-то образом прошло открытие нефискального документа.");
            return false;
        }

        environment()->logger()->log("Прокручиваем чек.");

        environment()->driver()->scrolling(
                password,
                7,
                4
        );

        environment()->logger()->log("Делаем полную отрезку.");
        if (!environment()->driver()->cutCheck(password, 1))
        {
            environment()->logger()->log(
                    "Отрезка чека не удалась."
            );

            return false;
        }

//        environment()->logger()->log("Получаем короткий запрос состояния.");
//        for (uint32_t times = 0; times < 20; ++times)
//        {
//            environment()->driver()->shortStateRequest(password);
//            if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
//            {
//                environment()->logger()->log("Не удалось получить короткий запрос состояния.");
//                return false;
//            }
//        }

//        environment()->logger()->log(std::to_string(cycle) + " чек напечатан в буффер. Ждем 1 секунду.");
//        std::this_thread::sleep_for(
//                std::chrono::milliseconds(800)
//        );

    }

    return true;
}

bool NonfiscalRequesting::print(const std::string &text, uint8_t font)
{
    if (getValue("Font print").toBoolean() || font != 1)
    {
        return environment()->driver()->fontStringPrint(
                getValue("Password").toUInt32(),
                7,
                font,
                text
        );
    }
    else
    {
        return environment()->driver()->standardStringPrint(
                getValue("Password").toUInt32(),
                7,
                text
        );
    }
}
