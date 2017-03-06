//
// Created by megaxela on 11.01.17.
//

#ifndef FRTESTER_CHECKLOADERTEST_H
#define FRTESTER_CHECKLOADERTEST_H


#include <include/Testing/AbstractTest.h>

class CheckLoaderTest : public AbstractTest
{
public:
    CheckLoaderTest();

    CheckLoaderTest(TestEnvironment* environment);

    bool execute() override;
};


#endif //FRTESTER_CHECKLOADERTEST_H
