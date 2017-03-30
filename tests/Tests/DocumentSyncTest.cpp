//
// Created by megaxela on 30.03.17.
//

#include <Testing/StaticTestFabric.h>
#include "DocumentSyncTest.h"

REGISTER_STATIC_TEST(DocumentSyncTest)

DocumentSyncTest::DocumentSyncTest() :
    AbstractTest(nullptr,
                 "Стабильный тест для проверки синхронизации документов",
                 "",
                 {{"Password", (uint32_t) 30},
                  {"Checks in change", (uint32_t) 20}})
{

}

bool DocumentSyncTest::execute()
{

}
