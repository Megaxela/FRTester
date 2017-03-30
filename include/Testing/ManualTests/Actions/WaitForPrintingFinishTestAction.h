//
// Created by megaxela on 13.03.17.
//

#ifndef FRTESTER_WAITFORPRINTINGFINISHACTION_H
#define FRTESTER_WAITFORPRINTINGFINISHACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class WaitForPrintingFinishTestAction : public AbstractTestAction
{
public:

    WaitForPrintingFinishTestAction();

    ~WaitForPrintingFinishTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_WAITFORPRINTINGFINISHACTION_H
