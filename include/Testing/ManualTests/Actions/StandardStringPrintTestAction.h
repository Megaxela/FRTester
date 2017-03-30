//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_STANDARDSTRINGPRINTTESTACTION_H
#define FRTESTER_STANDARDSTRINGPRINTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class StandardStringPrintTestAction : public AbstractTestAction
{
public:
    StandardStringPrintTestAction();

    ~StandardStringPrintTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_STANDARDSTRINGPRINTTESTACTION_H
