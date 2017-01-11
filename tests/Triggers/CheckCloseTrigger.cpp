//
// Created by megaxela on 10.01.17.
//

#include "CheckCloseTrigger.h"
#include <algorithm>
#include <include/Tools/ByteArrayReader.h>

CheckCloseTrigger::CheckCloseTrigger(TestEnvironment *environment) :
    AbstractTriggerTest(
            environment,
            "Триггер закрытия чека",
            "Триггер контроллирующий правильность оперирования"
            "регистрами при закрытии чека.",
            true
    ),
    m_tags({"close_check"}),
    m_success(false)
{

}

void CheckCloseTrigger::onPreExecute(const std::string &realTag, const ByteArray &arguments)
{
    m_success = false;
    parseArguments(arguments);


}

void CheckCloseTrigger::onPostExecute()
{

}

bool CheckCloseTrigger::succeed()
{
    return m_success;
}

bool CheckCloseTrigger::containsTag(const std::string &tag)
{
    return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
}

void CheckCloseTrigger::parseArguments(const ByteArray &data)
{
    ByteArrayReader reader(data);
    m_argPwd = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);
    m_cashSum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
    m_type2Sum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
    m_type3Sum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
    m_type4Sum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
}
