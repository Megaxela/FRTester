//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PINGTESTACTION_H
#define FRTESTER_PINGTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class PingTestAction : public AbstractTestAction
{
public:
    PingTestAction();

    ~PingTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PINGTESTACTION_H
