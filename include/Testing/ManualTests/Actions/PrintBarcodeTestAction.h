//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_PRINTBARCODETESTACTION_H
#define FRTESTER_PRINTBARCODETESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class PrintBarcodeTestAction : public AbstractTestAction
{
public:
    PrintBarcodeTestAction();

    ~PrintBarcodeTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_PRINTBARCODETESTACTION_H
