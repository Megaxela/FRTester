//
// Created by megaxela on 12.01.17.
//

#ifndef FRTESTER_WRITESHITCASHIERNAMETEST_H
#define FRTESTER_WRITESHITCASHIERNAMETEST_H


#include <include/Testing/AbstractTest.h>

class WriteShitCashierNameTest : public AbstractTest
{
public:
    WriteShitCashierNameTest();

    WriteShitCashierNameTest(TestEnvironment* environment);

    bool execute() override;
};


#endif //FRTESTER_WRITESHITCASHIERNAMETEST_H
