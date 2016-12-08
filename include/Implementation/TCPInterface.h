//
// Created by megaxela on 08.12.16.
//

#ifndef FRTESTER_TCPINTERFACE_H
#define FRTESTER_TCPINTERFACE_H


#include <include/Tools/NetworkTools.h>
#include <include/Tools/IPv4Address.h>
#include <include/Abstract/PhysicalInterface.h>

/**
 * @brief Класс, описывающий интерфейс соединения по TCP порту.
 */
class TCPInterface : public PhysicalInterface
{
public:
    static const int Type;

    /**
     * @brief Конструктор.
     */
    TCPInterface();

    /**
     * @brief Деструктор.
     */
    ~TCPInterface();

    /**
     * @brief Метод для открытия соединения с TCP сервером.
     * @return Успешность соединения.
     */
    bool openConnection() override;

    /**
     * @brief Метод закрытия соединения по COM порту.
     * @return Успешность закрытия соединения.
     */
    bool closeConnection() override;

    /**
     * @brief Метод для установки адреса для передачи данных
     * по TCP соединению.
     * @param data Данные.
     * @return Количество отправленных данных.
     */
    size_t write(const ByteArray &data) override;

    /**
     * @brief Метод для считывания данных из TCP стека.
     * @param n Количество считываемых данных.
     * @param timeout Таймаут на чтение.
     * @return Результат считывания.
     */
    ByteArray read(const size_t &n, uint32_t timeout) override;

    /**
     * @brief Метод для установки адреса для соединения
     * с устройством.
     * @param address IPv4 адрес.
     * @param port Порт.
     */
    void setAddress(const IPv4Address &address, uint16_t port);

    /**
     * @brief Метод для получения текущего установленного
     * адреса.
     * @return Текущий адрес.
     */
    IPv4Address address() const;

    /**
     * @brief Метод для получения текущего установленного
     * порта.
     * @return Текущий порт.
     */
    uint16_t port() const;

private:
    NetworkTools::socket_t m_connectionSocket;

    IPv4Address m_address;
    uint16_t m_port;


};


#endif //FRTESTER_TCPINTERFACE_H
