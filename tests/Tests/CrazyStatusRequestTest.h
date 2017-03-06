//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_CRAZYSTATUSREQUESTTEST_H
#define FRTESTER_CRAZYSTATUSREQUESTTEST_H


#include <include/Testing/AbstractTest.h>

class CrazyStatusRequestTest : public AbstractTest
{
public:

    CrazyStatusRequestTest();

    /**
     * @brief Конструктор.
     */
    CrazyStatusRequestTest(TestEnvironment* environment);

    virtual bool execute();
};


#endif //FRTESTER_CRAZYSTATUSREQUESTTEST_H
