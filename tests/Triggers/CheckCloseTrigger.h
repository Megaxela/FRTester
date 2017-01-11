//
// Created by megaxela on 10.01.17.
//

#ifndef FRTESTER_CHECKCLOSETRIGGER_H
#define FRTESTER_CHECKCLOSETRIGGER_H


#include <include/Testing/AbstractTriggerTest.h>

class CheckCloseTrigger : public AbstractTriggerTest
{
public:
    CheckCloseTrigger(TestEnvironment* environment);

    void onPreExecute(const std::string &realTag, const ByteArray &arguments) override;

    void onPostExecute() override;

    bool succeed() override;

    bool containsTag(const std::string &tag) override;

private:
    void parseArguments(const ByteArray &data);

    uint32_t m_argPwd;
    uint64_t m_cashSum;
    uint64_t m_type2Sum;
    uint64_t m_type3Sum;
    uint64_t m_type4Sum;

    std::vector<std::string> m_tags;
    bool m_success;
};


#endif //FRTESTER_CHECKCLOSETRIGGER_H
