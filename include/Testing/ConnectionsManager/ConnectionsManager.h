//
// Created by megaxela on 05.04.17.
//

#ifndef FRTESTER_CONNECTIONSMANAGER_H
#define FRTESTER_CONNECTIONSMANAGER_H

#include <Implementation/COMInterface.h>
#include <Implementation/TCPInterface.h>

class Connection;

/**
 * @brief Синглетон, контроллирующий доступные соединения.
 */
class ConnectionsManager
{
public:

    /**
     * @brief Деструктор.
     */
    ~ConnectionsManager();

    /**
     * @brief Метод для получения экземпляра класса.
     * @return Ссылка на экземпляр класса.
     */
    static ConnectionsManager& instance();

    /**
     * @brief Метод для получения интерфейса для
     * обмена по COM порту.
     * @return Умный указатель на интерфейс обмена по COM порту.
     */
    std::shared_ptr<COMInterface> getCOMInterface() const;

    /**
     * @brief Метод для получения интерфейса для
     * обмена по TCP.
     * @return Умный указатель на интерфейс обмена по TCP.
     */
    std::shared_ptr<TCPInterface> getTCPInterface() const;

    /**
     * @brief Метод для получения количества доступных
     * соединений.
     * @return Количество доступных соединений.
     */
    uint32_t numberOfConnections();

    /**
     * @brief Метод для получения соединения по индексу.
     * @param index Индекс соединения. Должен быть меньше,
     * чем numberOfConnections.
     * @return Умный указатель на соединение.
     */
    std::shared_ptr<Connection> getConnection(uint32_t index);



    /**
     * @brief Метод для удаления соединения по индексу.
     * @param index Индекс удаляемого соединения.
     */
    void removeConnection(uint32_t index);

private:
    ConnectionsManager();

    void loadConnections();

    void saveConnections();

    std::shared_ptr<COMInterface> m_comInterface;
    std::shared_ptr<TCPInterface> m_tcpInterface;

    std::vector<std::shared_ptr<Connection>> m_connections;
};


#endif //FRTESTER_CONNECTIONSMANAGER_H
