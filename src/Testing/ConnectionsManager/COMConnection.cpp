//
// Created by megaxela on 05.04.17.
//

#include "include/Testing/ConnectionsManager/COMConnection.h"
#include "include/Testing/ConnectionsManager/ConnectionsManager.h"
#include <FRDriver.h>
#include <Tools/Logger.h>

COMConnection::COMConnection() :
        Connection(Type::COM),
        m_device(),
        m_speed(0)
{

}

COMConnection::~COMConnection()
{

}

void COMConnection::setDevice(const std::string &s)
{
    m_device = s;
}

std::string COMConnection::device() const
{
    return m_device;
}

void COMConnection::setSpeed(uint32_t speed)
{
    m_speed = speed;
}

uint32_t COMConnection::speed() const
{
    return m_speed;
}

std::string COMConnection::name() const
{
    return "COM (\"" + m_device + "\", " + std::to_string(m_speed) + ")";
}

bool COMConnection::performConnection(FRDriver* driver)
{
    LogStream() << "Пробуем подключиться по соединению \"" << name() << "\"." << std::endl;
    auto comInterface = ConnectionsManager::instance().getCOMInterface();
    driver->setInterface(comInterface);

    comInterface->setBaudRate(m_speed);
    comInterface->setDevice(m_device);

    if (!comInterface->openConnection())
    {
        LogStream() << "Не удалось открыть соединение." << std::endl;
        return false;
    }

    if (!driver->checkConnection())
    {
        LogStream() << "Открыть соединение удалось, но проверить соединение не вышло." << std::endl;
        return false;
    }

    return true;
}

json COMConnection::toJson() const
{
    json result;

    result["type"] = "COM";
    result["device"] = m_device;
    result["speed"] = m_speed;

    return result;
}

std::shared_ptr<COMConnection> COMConnection::fromJson(json j)
{
    if (j.find("type") == j.end())
    {
        ErrorEx("COMConnection", "Попытка обработать ложное COM соединение.");
        return nullptr;
    }

    if (j["type"] != "COM")
    {
        ErrorStreamEx("COMConnection") << "Попытка обработать соединение с типом \""
                                       << j["type"]
                                       << "\" как COM."
                                       << std::endl;
        return nullptr;
    }

    auto connection = std::make_shared<COMConnection>();

    connection->setDevice(j["device"]);
    connection->setSpeed(j["speed"]);

    return connection;
}
