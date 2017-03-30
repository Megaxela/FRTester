//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_TAXESREPORTTESTACTION_H
#define FRTESTER_TAXESREPORTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class TaxesReportTestAction : public AbstractTestAction
{
public:
    TaxesReportTestAction();

    ~TaxesReportTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_TAXESREPORTTESTACTION_H
