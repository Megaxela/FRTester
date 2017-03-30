//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_CUTCHECKTESTACTION_H
#define FRTESTER_CUTCHECKTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class CutCheckTestAction : public AbstractTestAction
{
public:
    CutCheckTestAction();

    ~CutCheckTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_CUTCHECKTESTACTION_H
