//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_FONTSTRINGPRINTTESTACTION_H
#define FRTESTER_FONTSTRINGPRINTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class FontStringPrintTestAction : public AbstractTestAction
{
public:

    FontStringPrintTestAction();

    ~FontStringPrintTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_FONTSTRINGPRINTTESTACTION_H
