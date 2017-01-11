//
// Created by megaxela on 09.01.17.
//

#ifndef FRTESTER_OPERATIONTRIGGER_H
#define FRTESTER_OPERATIONTRIGGER_H


#include <include/Testing/AbstractTriggerTest.h>

class OperationTrigger : public AbstractTriggerTest
{
public:
    OperationTrigger(TestEnvironment *environment);

    void onPreExecute(const std::string &realTag, const ByteArray &arguments) override;

    void onPostExecute() override;

    bool succeed() override;

    bool containsTag(const std::string &tag) override;

private:
    void parseArguments(const ByteArray& arguments);

    uint32_t tagToIndex(const std::string &tag);

    std::vector<std::string> m_tags;

    std::string m_tag;

    uint32_t m_usingPwd;

    uint32_t m_pwd;
    uint64_t m_count;
    uint64_t m_value;
    uint8_t m_dep;

    uint64_t m_moneyRegistersOperationsByDepartment;
    uint32_t m_operatingRegisterOperations;

    bool m_success;
};


#endif //FRTESTER_OPERATIONTRIGGER_H
