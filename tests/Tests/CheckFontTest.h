//
// Created by megaxela on 07.02.17.
//

#ifndef FRTESTER_CHECKFONTTEST_H
#define FRTESTER_CHECKFONTTEST_H


#include <include/Testing/AbstractTest.h>

class CheckFontTest : public AbstractTest
{
public:
    CheckFontTest();

    CheckFontTest(TestEnvironment* testEnvironment);

    bool execute() override;

private:
    uint8_t readFont(uint32_t pwd, uint8_t field);

    bool writeFont(uint32_t pwd, uint8_t field, uint8_t value);
};


#endif //FRTESTER_CHECKFONTTEST_H
