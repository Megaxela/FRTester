//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_SECTIONSREPORTTESTACTION_H
#define FRTESTER_SECTIONSREPORTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class SectionsReportTestAction : public AbstractTestAction
{
public:

    SectionsReportTestAction();

    ~SectionsReportTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SECTIONSREPORTTESTACTION_H
