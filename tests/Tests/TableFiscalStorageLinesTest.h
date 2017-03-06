//
// Created by megaxela on 30.01.17.
//

#ifndef FRTESTER_TABLEFISCALSTORAGELINESTEST_H
#define FRTESTER_TABLEFISCALSTORAGELINESTEST_H


#include <include/Testing/AbstractTest.h>

class TableFiscalStorageLinesTest : public AbstractTest
{
public:
    TableFiscalStorageLinesTest();

    TableFiscalStorageLinesTest(TestEnvironment* environment);

    bool execute() override;
};


#endif //FRTESTER_TABLEFISCALSTORAGELINESTEST_H
