//
// Created by megaxela on 30.01.17.
//

#include "TableFiscalStorageLinesTest.h"
#include <Testing/TestLogger.h>

TableFiscalStorageLinesTest::TableFiscalStorageLinesTest(TestEnvironment* environment) :
    AbstractTest(
            environment,
            "Проверка полей таблицы Fiscal Storage.",
            "Тест проверяет поля, длиной в 128 символов в "
            "таблице FiscalStorage.",
            {{"Password", (uint32_t) 30}})
{

}

bool TableFiscalStorageLinesTest::execute()
{
    uint32_t pwd = getValueUInt32("Password");

    if (!enviroment()->driver()->writeTable(
            pwd,
            18,
            9,
            2,
            std::string(128, 'H')
    ))
    {
        enviroment()->logger()->log("Не удалось записать длинную строку в таблицу.");
        return true;
    }

    return enviroment()->driver()->openShift(pwd);
}
