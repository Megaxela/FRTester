//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_DOCUMENTHEADERPRINTTESTACTION_H
#define FRTESTER_DOCUMENTHEADERPRINTTESTACTION_H


#include <Testing/ManualTests/AbstractTestAction.h>

class DocumentHeaderPrintTestAction : public AbstractTestAction
{
public:
    DocumentHeaderPrintTestAction();

    ~DocumentHeaderPrintTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_DOCUMENTHEADERPRINTTESTACTION_H
