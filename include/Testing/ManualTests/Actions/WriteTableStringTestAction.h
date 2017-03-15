//
// Created by megaxela on 15.03.17.
//

#ifndef FRTESTER_WRITETABLETESTACTION_H
#define FRTESTER_WRITETABLETESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class WriteTableStringTestAction : public AbstractTestAction
{
public:
    WriteTableStringTestAction();
    
    ~WriteTableStringTestAction();
    
    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_WRITETABLETESTACTION_H
