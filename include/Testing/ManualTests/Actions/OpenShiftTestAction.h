//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_OPENSHIFTTESTACTION_H
#define FRTESTER_OPENSHIFTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class OpenShiftTestAction : public AbstractTestAction
{
public:
    OpenShiftTestAction();

    ~OpenShiftTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_OPENSHIFTTESTACTION_H
