//
// Created by megaxela on 26.01.17.
//

#ifndef FRTESTER_NONFISCALREQUESTING_H
#define FRTESTER_NONFISCALREQUESTING_H


#include <Testing/AbstractTest.h>

class NonfiscalRequesting : public AbstractTest
{
public:
    NonfiscalRequesting();

    /**
     * @brief Constructor.
     */
    NonfiscalRequesting(TestEnvironment* environment);

    bool execute() override;

private:
    bool print(const std::string& text, uint8_t font);

};


#endif //FRTESTER_NONFISCALREQUESTING_H
