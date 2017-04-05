//
// Created by megaxela on 05.04.17.
//

#include <Tools/Logger.h>
#include <Testing/ConnectionsManager/COMConnection.h>
#include <Testing/ConnectionsManager/TCPConnection.h>
#include "Testing/ConnectionsManager/Connection.h"

Connection::Connection(Type type) :
    m_type(type)
{

}

Connection::~Connection()
{

}

Connection::Type Connection::type() const
{
    return m_type;
}

std::shared_ptr<Connection> Connection::fromJson(json j)
{
    if (j.find("type") == j.end())
    {
        ErrorEx("Connection", "Попытка обработать соединение без поля типа.");
        return nullptr;
    }

    std::string type = j["type"];

    if (type == "COM")
    {
        return COMConnection::fromJson(j);
    }
    else if (type == "TCP")
    {
        return TCPConnection::fromJson(j);
    }

    ErrorEx("Connection", "Неизвестный тип соединения.");
    return nullptr;
}
