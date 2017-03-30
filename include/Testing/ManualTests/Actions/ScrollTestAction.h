//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_SCROLLTESTACTION_H
#define FRTESTER_SCROLLTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class ScrollTestAction : public AbstractTestAction
{
public:

    ScrollTestAction();

    ~ScrollTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SCROLLTESTACTION_H
