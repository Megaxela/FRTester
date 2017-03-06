//
// Created by megaxela on 23.02.17.
//

#ifndef FRTESTER_MAGNITCHECKTEST_H
#define FRTESTER_MAGNITCHECKTEST_H


#include <include/Testing/TestEnvironment.h>
#include <include/Testing/AbstractTest.h>

class MagnitCheckTest : public AbstractTest
{
public:
    MagnitCheckTest();

    MagnitCheckTest(TestEnvironment* environment);

    bool execute() override;
};


#endif //FRTESTER_MAGNITCHECKTEST_H
