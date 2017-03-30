//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PAYOUTTESTACTION_H
#define FRTESTER_PAYOUTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class PayoutTestAction : public AbstractTestAction
{
public:
    PayoutTestAction();

    ~PayoutTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PAYOUTTESTACTION_H
