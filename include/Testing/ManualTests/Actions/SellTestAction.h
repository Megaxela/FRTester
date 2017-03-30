//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_SELLTESTACTION_H
#define FRTESTER_SELLTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class SellTestAction : public AbstractTestAction
{
public:
    SellTestAction();

    ~SellTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SELLTESTACTION_H
