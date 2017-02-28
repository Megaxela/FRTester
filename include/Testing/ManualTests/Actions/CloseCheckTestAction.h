//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_CLOSECHECKTESTACTION_H
#define FRTESTER_CLOSECHECKTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class CloseCheckTestAction : public AbstractTestAction
{
public:

    CloseCheckTestAction();

    ~CloseCheckTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_CLOSECHECKTESTACTION_H
