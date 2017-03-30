//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_REBOOTTESTACTION_H
#define FRTESTER_REBOOTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class RebootTestAction : public AbstractTestAction
{
public:

    RebootTestAction();

    ~RebootTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_REBOOTTESTACTION_H
