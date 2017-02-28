//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PRINTCLICHES_H
#define FRTESTER_PRINTCLICHES_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class PrintClichesTestAction : public AbstractTestAction
{
public:

    PrintClichesTestAction();

    ~PrintClichesTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PRINTCLICHES_H
