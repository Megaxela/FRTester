//
// Created by megaxela on 05.04.17.
//

#include <fstream>
#include "Testing/ConnectionsManager/ConnectionsManager.h"
#include <json.hpp>
#include <shared_libs/frdrvcross/include/Tools/Logger.h>
#include <include/Testing/ConnectionsManager/Connection.h>

using nlohmann::json;

ConnectionsManager::ConnectionsManager() :
    m_comInterface(std::make_shared<COMInterface>()),
    m_tcpInterface(std::make_shared<TCPInterface>()),
    m_connections()
{
    loadConnections();
}

ConnectionsManager::~ConnectionsManager()
{
    saveConnections();
}

ConnectionsManager &ConnectionsManager::instance()
{
    static ConnectionsManager manager;
    return manager;
}

std::shared_ptr<COMInterface> ConnectionsManager::getCOMInterface() const
{
    return m_comInterface;
}

std::shared_ptr<TCPInterface> ConnectionsManager::getTCPInterface() const
{
    return m_tcpInterface;
}

uint32_t ConnectionsManager::numberOfConnections()
{
    return static_cast<uint32_t>(m_connections.size());
}

std::shared_ptr<Connection> ConnectionsManager::getConnection(uint32_t index)
{
    return m_connections[index];
}

void ConnectionsManager::removeConnection(uint32_t index)
{
    m_connections.erase(m_connections.begin() + index);
    saveConnections();
}

void ConnectionsManager::loadConnections()
{
    std::ifstream fl("connections.json");

    if (!fl.is_open())
    {
        return;
    }

    json connections = json::parse(fl);

    if (connections.find("connections") == connections.end())
    {
        Error("Попытка загрузить файл соединений без корневого поля \"connections\".");
        return;
    }

    connections = connections["connections"];

    for (auto& connection : connections)
    {
        auto connectionObject = Connection::fromJson(connection);

        if (connectionObject == nullptr)
        {
            Warning("Не удалось загрузить соединение.");
            continue;
        }

        m_connections.push_back(connectionObject);
    }

    Log("Загрузка соединений завершена.");
}

void ConnectionsManager::saveConnections()
{
    std::ofstream fl("connections.json");

    if (!fl.is_open())
    {
        Error("Не удалось открыть файл для сохранения соединений.");
        return;
    }

    json connectionArray;

    for (auto& connection : m_connections)
    {
        connectionArray.push_back(connection->toJson());
    }

    json result;
    result["connections"] = connectionArray;

    fl << result.dump(4);
    fl.close();
}
