//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_SHIFTREPORTWITHOUTEXTINCTIONTESTACTION_H
#define FRTESTER_SHIFTREPORTWITHOUTEXTINCTIONTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class ShiftReportWithoutExtinctionTestAction : public AbstractTestAction
{
public:
    ShiftReportWithoutExtinctionTestAction();

    ~ShiftReportWithoutExtinctionTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SHIFTREPORTWITHOUTEXTINCTIONTESTACTION_H
