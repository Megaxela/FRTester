//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PAYINTESTACTION_H
#define FRTESTER_PAYINTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class PayinTestAction : public AbstractTestAction
{
public:

    PayinTestAction();

    ~PayinTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PAYINTESTACTION_H
