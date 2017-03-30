//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_BUYTESTACTION_H
#define FRTESTER_BUYTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class BuyTestAction : public AbstractTestAction
{
public:
    BuyTestAction();

    ~BuyTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_BUYTESTACTION_H
