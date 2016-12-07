//
// Created by megaxela on 21.11.16.
//

#include <include/DriverHolder.h>
#include <map>
#include <include/Testing/TestCore.h>
#include <Tools/Logger.h>
#include "include/Testing/TestDriver.h"

TestDriver::TestDriver()
{

}

TestDriver::~TestDriver()
{

}

ByteArray TestDriver::sendCommand(const FRDriver::Command &c, const ByteArray &arguments)
{
    auto tag = getCommandTag(c);

    if (tag.empty())
    {
        return FRDriver::sendCommand(c, arguments);
    }

    m_currentTriggers.clear();
    TestCore::instance().getTriggers(tag, m_currentTriggers);

    for (auto trigger : m_currentTriggers)
    {
        trigger->onPreExecute(tag);
    }

    Log("Вызываем команду " + std::to_string((int) c) + " с тэгом \"" + tag + "\"");

    auto result = FRDriver::sendCommand(c, arguments);

    if (getLastError() == FRDriver::ErrorCode::NoError)
    {
        Log("Команда выполнилась без ошибок.");
    }
    else
    {
        Log("Команда выполнилась с ошибкой №" +
            std::to_string((int) getLastError()) +
            " - " +
            FRDriver::Converters::errorToString((int) getLastError()))
    }

    for (auto trigger : m_currentTriggers)
    {
        trigger->onPostExecute();
        if (!trigger->succeed())
        {
            TestCore::instance().triggerMistaken(trigger);
        }
    }

    return result;
}



std::string TestDriver::getCommandTag(const FRDriver::Command &c)
{
    static std::map<FRDriver::Command, std::string> commandTags = {
            {Command::Beep, "beep"},
            {Command::Sell, "sell"},
            {Command::ShortStateRequest, "short_state_request"},
            {Command::FullStateRequest, "full_state_request"},
            {Command::OpenShift, "open_shift"},
            {Command::ShiftCloseReport, "z_report"},
            {Command::ContinuePrint, "continue_print"},
            {Command::NullifyCheck, "cancel_check"},
            {Command::CloseCheck, "close_check"},
            {Command::OperatingRegisterRequest, ""}
    };

    auto pos = commandTags.end();
    if ((pos = commandTags.find(c)) != commandTags.end())
    {
        return pos->second;
    }

    return "";
}

FRDriver *TestDriver::pureDriver()
{
    return &DriverHolder::driver();
}
