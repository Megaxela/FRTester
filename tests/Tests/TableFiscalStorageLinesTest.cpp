//
// Created by megaxela on 30.01.17.
//

#include "TableFiscalStorageLinesTest.h"
#include <Testing/TestLogger.h>
#include <Testing/StaticTestFabric.h>

REGISTER_STATIC_TEST(TableFiscalStorageLinesTest)

TableFiscalStorageLinesTest::TableFiscalStorageLinesTest() :
    TableFiscalStorageLinesTest(nullptr)
{

}

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

    if (!environment()->driver()->writeTable(
            pwd,
            18,
            1,
            9,
            std::string(128, 'H')
    ))
    {
        environment()->logger()->log("Не удалось записать длинную строку в таблицу.");
        return true;
    }

    return environment()->driver()->openShift(pwd);
}
