//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_MESSAGEBOXTEST_H
#define FRTESTER_MESSAGEBOXTEST_H


#include <include/Testing/AbstractTest.h>

class MessageBoxTest : public AbstractTest
{
public:
    MessageBoxTest(TestEnvironment* testEnvironment);

    virtual bool execute();

};


#endif //FRTESTER_MESSAGEBOXTEST_H