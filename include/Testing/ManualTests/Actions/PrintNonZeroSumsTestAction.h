//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PRINTNONZEROSUMSTESTACTION_H
#define FRTESTER_PRINTNONZEROSUMSTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class PrintNonZeroSumsTestAction : public AbstractTestAction
{
public:
    PrintNonZeroSumsTestAction();

    ~PrintNonZeroSumsTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PRINTNONZEROSUMSTESTACTION_H
