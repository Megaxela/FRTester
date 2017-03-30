//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_RETURNSELLTESTACTION_H
#define FRTESTER_RETURNSELLTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class ReturnSellTestAction : public AbstractTestAction
{
public:

    ReturnSellTestAction();

    ~ReturnSellTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_RETURNSELLTESTACTION_H
