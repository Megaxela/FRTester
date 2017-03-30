//
// Created by megaxela on 21.11.16.
//

#include <map>
#include <Tools/Logger.h>
#include <DriverHolder.h>
#include <Testing/TestCore.h>
#include "Testing/TestDriver.h"
#include <Testing/TestLogger.h>

TestDriver::TestDriver() :
    m_commandCallback()
{

}

TestDriver::~TestDriver()
{

}

ByteArray TestDriver::sendCommand(const FRDriver::Command &c, const ByteArray &arguments, bool responseHasCashier)
{
    auto tag = getCommandTag(c);

    if (tag.empty())
    {
        return FRDriver::sendCommand(c, arguments, responseHasCashier);
    }

    std::vector<TriggerTestPtr> currentTriggers =
            TestCore::instance().getActiveTriggers();

    for (auto trigger : currentTriggers)
    {
        if (!trigger->containsTag(tag))
        {
            continue;
        }

        trigger->environment()->logger()->log(
                "Активируется триггер \"" +
                trigger->name() +
                "\"..."
        );
        trigger->onPreExecute(tag, arguments);
    }

    Log("Вызываем команду " + std::to_string((int) c) + " с тэгом \"" + tag + "\"");

    auto result = FRDriver::sendCommand(c, arguments, false);

    if (m_commandCallback)
    {
        m_commandCallback(c);
    }

    if (getLastError() == FRDriver::ErrorCode::NoError)
    {
        Log("Команда выполнилась без ошибок.");

        for (auto trigger : currentTriggers)
        {
            if (!trigger->containsTag(tag))
            {
                continue;
            }

            trigger->onPostExecute();
            if (!trigger->succeed())
            {
                TestCore::instance().triggerMistaken(trigger);
            }
        }
    }
    else
    {
        Log("Команда выполнилась с ошибкой №" +
            std::to_string((int) getLastError()) +
            " - " +
            FRDriver::Converters::errorToString((int) getLastError()))
    }

    return result;
}



std::string TestDriver::getCommandTag(const FRDriver::Command &c)
{
    static std::map<FRDriver::Command, std::string> commandTags = {
            {Command::ShortStateRequest,            "short_state_request"},
            {Command::FullStateRequest,             "full_state_request"},
            {Command::Beep,                         "beep"},
            {Command::ReadExchangeConfiguration,    "read_exchange_configuration"},
            {Command::TechReset,                    "tech_reset"},
            {Command::StandardStringPrint,          "standard_string_print"},
            {Command::DocumentHeaderPrint,          "document_header_print"},
            {Command::CurrencyRegisterRequest,      "currency_register_request"},
            {Command::OperatingRegisterRequest,     "operating_register_request"},
            {Command::WriteTable,                   "write_table"},
            {Command::ReadTable,                    "read_table"},
            {Command::TimeProgramming,              "time_programming"},
            {Command::DateProgramming,              "date_programming"},
            {Command::DateConfirm,                  "date_confirm"},
            {Command::TableValuesInit,              "table_values_init"},
            {Command::CutCheck,                     "cut_check"},
            {Command::ReadFontConfiguration,        "read_font_configuration"},
            {Command::TotalExtinction,              "total_extinction"},
            {Command::Scrolling,                    "scrolling"},
            {Command::TableStructureRequest,        "table_structure_request"},
            {Command::ShiftReportNoExtinction,      "shift_report_no_extinction"},
            {Command::ShiftCloseReport,             "shift_close_report"},
            {Command::Sell,                         "sell"},
            {Command::Buy,                          "buy"},
            {Command::ReturnSell,                   "return_sell"},
            {Command::ReturnBuy,                    "return_buy"},
            {Command::CloseCheck,                   "close_check"},
            {Command::CancelCheck,                  "cancel_check"},
            {Command::ContinuePrint,                "continue_print"},
            {Command::OpenShift,                    "open_shift"},
            {Command::FieldStructureRequest,        "field_structure_request"},
            {Command::FontStringPrint,              "font_string_print"},
            {Command::SectionsReport,               "sections_report"},
            {Command::TaxesReport,                  "taxes_report"},
            {Command::CashierReport,                "cashier_report"},
            {Command::PayIn,                        "pay_in"},
            {Command::PayOut,                       "pay_out"},
            {Command::PrintCliches,                 "print_cliches"},
            {Command::DocumentEndPrint,             "document_end_print"},
            {Command::PrintAds,                     "print_ads"},
            {Command::EnterFactoryNumber,           "enter_factory_number"},
            {Command::NonZeroSums,                  "non_zero_sums"},
            {Command::GetInformationExchangeStatus, "get_information_exchange_status"},
            {Command::CheckResult,                  "check_result"}
    };

    auto pos = commandTags.end();
    if ((pos = commandTags.find(c)) != commandTags.end())
    {
        return pos->second;
    }

    WarningStream() << "Для команды с кодом №" << (int) c << " не найден тэг." << std::endl;

    return "";
}

FRDriver *TestDriver::pureDriver()
{
    return &DriverHolder::driver();
}

void TestDriver::setCommandCallback(std::function<void(FRDriver::Command)> callback)
{
    m_commandCallback = callback;
}
