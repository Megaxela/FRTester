//
// Created by megaxela on 20.03.17.
//

#include <include/Testing/StaticTestFabric.h>
#include "Bug0017479Test.h"

REGISTER_STATIC_TEST(Bug0017479Test)

Bug0017479Test::Bug0017479Test() :
    AbstractTest(nullptr,
                 "Тест бага №0017479.",
                 "Если в таблице \"Региональные настройки\" включить печать отмененных документов и в поле \"Не печатать документ\" установить значение = 1 (если значение = 2, то все нормально), то при аннулировании чека срабатывает промотка и отрезка чека, а также печать заголовка для следующего документа",
                 {{"Password", (uint32_t) 30},
                  })
{

}

bool Bug0017479Test::execute()
{
    auto password = getValue("Password").toUInt32();

    // Открываем смену
    // Отключаем печать
    auto old = environment()->tools()->enablePrinting(password, TestingTools::Printing::Disabled);
    if (!environment()->driver()->openShift(password))
    {
        environment()->tools()->enablePrinting(password, old);

        environment()->logger()->log("Не удалось открыть смену.");
        return false;
    }
    environment()->tools()->enablePrinting(password, old);

    // Сохраняем старое значение таблицы
    // Печать отмененных документов
    auto startPrintCanceledDocuments = environment()->driver()->readTableBin(
            password, 17, 1, 15
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log("Не удалось получить начальное значение \"Печать отмененных документов.\"");
        return false;
    }

    // Не печатать документ
    auto startDontPrintDocument = environment()->driver()->readTableBin(
            password, 17, 1, 7
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log("Не удалось получить начальное значение \"Не печатать документ\".");
        return false;
    }

    // Устанавливаем новые значения
    if (!environment()->driver()->writeTable(
            password,
            17, // Региональные настройки
            1,
            15,  // Включить печать аннулированных чеков
            1,
            1
    ))
    {
        environment()->logger()->log("Не удалось записать значение в \"Печать аннулированных документов\".");
        return false;
    }

    if (!environment()->driver()->writeTable(
            password,
            17, // Региональные настройки
            1,
            7,  // Отключить печать документа
            1,
            1
    ))
    {
        environment()->logger()->log("Не удалось записать значение в \"Отключение печати\".");
        return false;
    }

    environment()->tools()->messageNotify("После нажатия ОК не должна срабатывать прокрутка и отрезка.");

    environment()->driver()->sell(
            password,
            1000,
            1000,
            1,
            0, 0, 0, 0,
            ""
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log("Не удалось пробить продажу.");

        return false;
    }

    environment()->driver()->cancelCheck(password);

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log("Не удалось отменить чек.");

        return false;
    }

    if (environment()->tools()->messageQuestion(
            "Отрезка и/или прокрутка случились?",
            "Да",
            "Нет"
    ))
    {
        environment()->logger()->log("А не должна была. Баг 0017479 воспроизвелся.");

        // Восстанавливаем старые значения
        if (!environment()->driver()->writeTable(
                password,
                17, // Региональные настройки
                1,
                15,  // Включить печать аннулированных чеков
                startPrintCanceledDocuments,
                1
        ))
        {
            environment()->logger()->log("Не удалось восстановить значение в \"Печать аннулированных документов\".");
            return false;
        }

        if (!environment()->driver()->writeTable(
                password,
                17, // Региональные настройки
                1,
                7,  // Отключить печать документа
                startDontPrintDocument,
                1
        ))
        {
            environment()->logger()->log("Не удалось восстановить значение в \"Отключение печати\".");
            return false;
        }

        return false;
    }

    // Восстанавливаем старые значения
    if (!environment()->driver()->writeTable(
            password,
            17, // Региональные настройки
            1,
            15,  // Включить печать аннулированных чеков
            startPrintCanceledDocuments,
            1
    ))
    {
        environment()->logger()->log("Не удалось восстановить значение в \"Печать аннулированных документов\".");
        return false;
    }

    if (!environment()->driver()->writeTable(
            password,
            17, // Региональные настройки
            1,
            7,  // Отключить печать документа
            startDontPrintDocument,
            1
    ))
    {
        environment()->logger()->log("Не удалось восстановить значение в \"Отключение печати\".");
        return false;
    }

    return true;
}
