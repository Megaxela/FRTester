//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_RETURNBUYTESTACTION_H
#define FRTESTER_RETURNBUYTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class ReturnBuyTestAction : public AbstractTestAction
{
public:
    ReturnBuyTestAction();

    ~ReturnBuyTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_RETURNBUYTESTACTION_H
