//
// Created by megaxela on 11.01.17.
//

#ifndef FRTESTER_CHECKLOADERTEST_H
#define FRTESTER_CHECKLOADERTEST_H


#include <include/Testing/AbstractTest.h>

class CheckLoaderTest : public AbstractTest
{
public:
    CheckLoaderTest(TestEnvironment* environment);

    bool execute() override;

private:
    uint32_t m_pwd;
    uint32_t m_numberOfChecks;
    uint32_t m_numberOfOperations;

    uint32_t m_goodPrice;
    uint32_t m_goodCount;
};


#endif //FRTESTER_CHECKLOADERTEST_H
