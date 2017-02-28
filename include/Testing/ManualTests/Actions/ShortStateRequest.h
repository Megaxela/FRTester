//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_SHORTSTATEREQUEST_H
#define FRTESTER_SHORTSTATEREQUEST_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class ShortStateRequest : public AbstractTestAction
{
public:

    ShortStateRequest();

    ~ShortStateRequest();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SHORTSTATEREQUEST_H
