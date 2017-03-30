//
// Created by megaxela on 15.03.17.
//

#ifndef FRTESTER_WRITETABLEBINTESTACTION_H
#define FRTESTER_WRITETABLEBINTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class WriteTableBinTestAction : public AbstractTestAction
{
public:
    WriteTableBinTestAction();

    ~WriteTableBinTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_WRITETABLEBINTESTACTION_H
