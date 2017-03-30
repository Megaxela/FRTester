//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_RESUMEPRINTINGTESTACTION_H
#define FRTESTER_RESUMEPRINTINGTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class ResumePrintingTestAction : public AbstractTestAction
{
public:
    ResumePrintingTestAction();

    ~ResumePrintingTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_RESUMEPRINTINGTESTACTION_H
