//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PRINTADSTESTACTION_H
#define FRTESTER_PRINTADSTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class PrintAdsTestAction : public AbstractTestAction
{
public:
    PrintAdsTestAction();

    ~PrintAdsTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PRINTADSTESTACTION_H
