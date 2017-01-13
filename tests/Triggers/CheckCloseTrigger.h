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

    uint32_t m_pwd;

    uint32_t m_argPwd;
    uint64_t m_cashSum;
    uint64_t m_type2Sum;
    uint64_t m_type3Sum;
    uint64_t m_type4Sum;

    uint64_t m_checkResult;

    uint32_t m_currentShiftNumber;
    uint32_t m_currentAction; // 0 - Продажа, 1 - Покупка, 2 - Возврат продажи, 3 - Возврат покупки
    uint64_t m_cashRegister193;
    uint64_t m_0to63MoneyRegister;
    uint64_t m_121to182MoneyRegister;

    uint64_t m_193to196PayMoneyRegister;
    uint64_t m_197to200PayMoneyRegister;
    uint64_t m_201to204PayMoneyRegister;
    uint64_t m_205to208PayMoneyRegister;

    uint64_t m_totalCashRegister241;

    std::vector<std::string> m_tags;
    bool m_success;

    bool m_willSucceed;
};


#endif //FRTESTER_CHECKCLOSETRIGGER_H
