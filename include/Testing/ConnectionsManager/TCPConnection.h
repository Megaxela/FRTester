//
// Created by megaxela on 05.04.17.
//

#ifndef FRTESTER_TCPCONNECTION_H
#define FRTESTER_TCPCONNECTION_H


#include "Connection.h"
#include <Tools/IPv4Address.h>

/**
 * @brief Соединение по TCP
 */
class TCPConnection : public Connection
{
public:
    /**
     * @brief Конструктор.
     */
    TCPConnection();

    /**
     * @brief Деструктор.
     */
    ~TCPConnection();

    /**
     * @brief Метод для установки адреса и порта
     * для соединения.
     * @param address Адрес.
     * @param port Порт
     */
    void setAddress(const IPv4Address& address, uint16_t port);

    /**
     * @brief Метод для получения адреса соединения.
     * @return Объект с IPv4 адресом.
     */
    IPv4Address address() const;

    /**
     * @brief Метод для получения порта соединения.
     * @return Порт.
     */
    uint16_t port() const;

    virtual std::string name() const;

    virtual bool performConnection(FRDriver *driver);

    virtual json toJson() const;

    /**
     * @brief Метод для получения объекта TCP соединения
     * из json структуры.
     * @param j JSON структура.
     * @return Умный указатель для TCP соединений.
     */
    static std::shared_ptr<TCPConnection> fromJson(json j);

private:
    IPv4Address m_address;
    uint16_t m_port;
};


#endif //FRTESTER_TCPCONNECTION_H
