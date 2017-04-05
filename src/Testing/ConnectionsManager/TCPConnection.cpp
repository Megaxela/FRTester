//
// Created by megaxela on 05.04.17.
//

#include <Tools/Logger.h>
#include "include/Testing/ConnectionsManager/TCPConnection.h"
#include "include/Testing/ConnectionsManager/ConnectionsManager.h"
#include <FRDriver.h>

TCPConnection::TCPConnection() :
    m_address(Type::TCP),
    m_port(0)
{

}

TCPConnection::~TCPConnection()
{

}

std::string TCPConnection::name() const
{
    return "TCP(" + m_address.toString() + ":" + std::to_string(m_port) + ")";
}

bool TCPConnection::performConnection(FRDriver *driver)
{
    LogStream() << "Пробуем подключиться по соединению \"" << name() << "\"." << std::endl;
    auto tcpInterface = ConnectionsManager::instance().getTCPInterface();

    tcpInterface->setAddress(m_address, m_port);

    if (!tcpInterface->openConnection())
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

void TCPConnection::setAddress(const IPv4Address &address, uint16_t port)
{
    m_address = address;
    m_port = port;
}

IPv4Address TCPConnection::address() const
{
    return m_address;
}

uint16_t TCPConnection::port() const
{
    return m_port;
}

json TCPConnection::toJson() const
{
    json result;

    result["type"] = "TCP";
    result["address"] = m_address.toString();
    result["port"] = m_port;

    return result;
}

std::shared_ptr<TCPConnection> TCPConnection::fromJson(json j)
{
    if (j.find("type") == j.end())
    {
        ErrorEx("TCPConnection", "Попытка обработать ложное TCP соединение.");
        return nullptr;
    }

    if (j["type"] != "TCP")
    {
        ErrorStreamEx("TCPConnection") << "Попытка обработать соединение с типом \""
                                       << j["type"]
                                       << "\" как TCP."
                                       << std::endl;
        return nullptr;
    }

    auto connection = std::make_shared<TCPConnection>();

    connection->setAddress(
            IPv4Address::fromString(j["address"]),
            j["port"]
    );

    return connection;
}
