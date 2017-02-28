//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_CASHIERREPORTTESTACTIONA_H
#define FRTESTER_CASHIERREPORTTESTACTIONA_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class CashierReportTestAction : public AbstractTestAction
{
public:
    CashierReportTestAction();

    ~CashierReportTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_CASHIERREPORTTESTACTION_H
