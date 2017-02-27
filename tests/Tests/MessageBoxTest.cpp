//
// Created by megaxela on 27.02.17.
//

#include "MessageBoxTest.h"

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
