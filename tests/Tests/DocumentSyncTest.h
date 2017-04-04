//
// Created by megaxela on 30.03.17.
//

#ifndef FRTESTER_DOCUMENTSYNCTEST_H
#define FRTESTER_DOCUMENTSYNCTEST_H


#include <Testing/AbstractTest.h>
#include <random>

/**
 * @brief Супер надежный тест для тестирования
 * стабильности синхронизации документов в ФН и
 * в ККТ при разных обстоятельствах.
 */
class DocumentSyncTest : public AbstractTest
{
public:
    DocumentSyncTest();

    bool execute() override;

private:
    std::random_device m_randomDevice;
    std::mt19937 m_rng;
};


#endif //FRTESTER_DOCUMENTSYNCTEST_H
