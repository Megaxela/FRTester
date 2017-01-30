//
// Created by megaxela on 26.01.17.
//

#ifndef FRTESTER_NONFISCALREQUESTING_H
#define FRTESTER_NONFISCALREQUESTING_H


#include <include/Testing/AbstractTest.h>

class NonfiscalRequesting : public AbstractTest
{
public:
    /**
     * @brief Constructor.
     */
    NonfiscalRequesting(TestEnvironment* environment);

    bool execute() override;

};


#endif //FRTESTER_NONFISCALREQUESTING_H
