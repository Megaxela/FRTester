//
// Created by megaxela on 21.02.17.
//

#ifndef FRTESTER_LONGSTATEREQUESTTESTACTION_H
#define FRTESTER_LONGSTATEREQUESTTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий длинный запрос состояния.
 */
class LongStateRequestTestAction : public AbstractTestAction
{
public:
    LongStateRequestTestAction();

    ~LongStateRequestTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_LONGSTATEREQUESTTESTACTION_H
