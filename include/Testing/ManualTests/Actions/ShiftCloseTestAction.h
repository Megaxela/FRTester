//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_SHIFTCLOSETESTACTION_H
#define FRTESTER_SHIFTCLOSETESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class ShiftCloseTestAction : public AbstractTestAction
{
public:

    ShiftCloseTestAction();

    ~ShiftCloseTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SHIFTCLOSETESTACTION_H
