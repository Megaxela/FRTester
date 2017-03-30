//
// Created by megaxela on 06.02.17.
//

#ifndef FRTESTER_BARCODEPRINTINGTEST_H
#define FRTESTER_BARCODEPRINTINGTEST_H


#include <Testing/AbstractTest.h>

class BarcodePrintingTest : public AbstractTest
{
public:
    BarcodePrintingTest();

    BarcodePrintingTest(TestEnvironment* environment);

    bool execute() override;
};


#endif //FRTESTER_BARCODEPRINTINGTEST_H
