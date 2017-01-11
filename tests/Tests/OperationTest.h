//
// Created by megaxela on 09.01.17.
//

#ifndef FRTESTER_OPERATIONTEST_H
#define FRTESTER_OPERATIONTEST_H


#include <include/Testing/AbstractTest.h>
#include <random>

class OperationTest : public AbstractTest
{
public:
    OperationTest(TestEnvironment *environment);

    bool execute() override;

private:
    uint32_t random(uint32_t begin, uint32_t end);

    uint32_t m_pwd;
    std::default_random_engine m_generator;
};


#endif //FRTESTER_OPERATIONTEST_H
