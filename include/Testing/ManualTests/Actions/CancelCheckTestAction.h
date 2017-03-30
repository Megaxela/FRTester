//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_CANCELCHECHTESTACTION_H
#define FRTESTER_CANCELCHECHTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class CancelCheckTestAction : public AbstractTestAction
{
public:
    CancelCheckTestAction();

    ~CancelCheckTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_CANCELCHECHTESTACTION_H
