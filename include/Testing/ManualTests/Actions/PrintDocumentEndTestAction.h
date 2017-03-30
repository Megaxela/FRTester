//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PRINTDOCUMENTEND_H
#define FRTESTER_PRINTDOCUMENTEND_H


#include <Testing/ManualTests/AbstractTestAction.h>

class PrintDocumentEndTestAction : public AbstractTestAction
{
public:
    PrintDocumentEndTestAction();

    ~PrintDocumentEndTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PRINTDOCUMENTEND_H
