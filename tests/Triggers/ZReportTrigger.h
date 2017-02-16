//
// Created by megaxela on 02.02.17.
//

#ifndef FRTESTER_ZREPORTTRIGGER_H
#define FRTESTER_ZREPORTTRIGGER_H


#include <include/Testing/AbstractTriggerTest.h>
#include <include/FRDriver.h>

/**
 * @brief Тест, проверяющий корректность выполнения Z отчета.
 */
class ZReportTrigger : public AbstractTriggerTest
{
public:
    /**
     * @brief Конструктор.
     * @param environment Тестовое окружение.
     */
    ZReportTrigger(TestEnvironment* environment);

    /**
     * @brief Метод, вызываемый перед выполнением команды.
     * @param realTag Тэг вызываемой команды.
     * @param arguments Аргументы команды.
     */
    void onPreExecute(const std::string &realTag, const ByteArray &arguments) override;

    /**
     * @brief Метод, вызываемый после выполнения команды.
     */
    void onPostExecute() override;

    /**
     * @brief Метод, показывающий успешность выполнения триггера.
     * @return Результат выполнения триггера.
     */
    bool succeed() override;

    /**
     * @brief Метод, проверяющий требуется ли вызывать этот триггер
     * для команды.
     * @param tag Тэг.
     * @return Вызывать ли этот триггер.
     */
    bool containsTag(const std::string &tag) override;

protected:
    bool m_success;
    bool m_preError;
    FRDriver::NonZeroSums m_nonZeroSums;
    struct {
        uint64_t incomingSum;
        uint64_t consumptionSum;
        uint64_t returnIncomingSum;
        uint64_t returnConsumptionSum;
    } m_countedSums;
};


#endif //FRTESTER_ZREPORTTRIGGER_H