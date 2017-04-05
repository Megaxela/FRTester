//
// Created by megaxela on 05.04.17.
//

#ifndef FRTESTER_COMCONNECTION_H
#define FRTESTER_COMCONNECTION_H


#include "Connection.h"

/**
 * @brief Соединение по COM порту.
 */
class COMConnection : public Connection
{
public:

    /**
     * @brief Конструктор.
     */
    COMConnection();

    /**
     * @brief Деструктор.
     */
    virtual ~COMConnection();

    /**
     * @brief Метод для установки устройства
     * для подключения.
     * @param s Название устройства.
     */
    void setDevice(const std::string& s);

    /**
     * @brief Метод для получения установленного
     * устройства для подключения.
     * @return Строковое представление устройства
     * для подключения.
     */
    std::string device() const;

    /**
     * @brief Метод для установки скорости обмена.
     * @param speed BaudRate
     */
    void setSpeed(uint32_t speed);

    /**
     * @brief Метод для получения скорости обмена.
     * @return BaudRate
     */
    uint32_t speed() const;

    virtual std::string name() const;

    virtual bool performConnection(FRDriver* driver);

    virtual json toJson() const;

    /**
     * @brief Метод для получения COM соединения из JSON
     * структуры.
     * @param j JSON структура.
     * @return Умный указатель с COM соединением.
     */
    static std::shared_ptr<COMConnection> fromJson(json j);

private:
    std::string m_device;
    uint32_t m_speed;
};


#endif //FRTESTER_COMCONNECTION_H
