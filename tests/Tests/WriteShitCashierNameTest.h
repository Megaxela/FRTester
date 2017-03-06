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

private:
    uint32_t m_sysAdmPwd;
    std::vector<uint32_t> m_cashierPasswords;
};


#endif //FRTESTER_WRITESHITCASHIERNAMETEST_H
