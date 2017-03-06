//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/StaticTestFabric.h>
#include "MessageBoxTest.h"

REGISTER_STATIC_TEST(MessageBoxTest);

MessageBoxTest::MessageBoxTest() :
    MessageBoxTest(nullptr)
{

}

MessageBoxTest::MessageBoxTest(TestEnvironment *testEnvironment) :
    AbstractTest(testEnvironment,
                 "Тест с модальными окнами",
                 "",
                 {{}})
{

}

bool MessageBoxTest::execute()
{
    while (environment()->tools()->messageQuestion(
            "Остановить?",
            "Продолжить",
            "Остановить"
    ));

    return true;
}
