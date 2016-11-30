//
// Created by megaxela on 11.11.16.
//

#ifndef FRDRVCROSS_PROTOCOL_H
#define FRDRVCROSS_PROTOCOL_H

#include <memory>
#include "../Tools/ByteArray.h"
#include "PhysicalInterface.h"

class Protocol;

typedef std::shared_ptr<Protocol> ProtocolPtr;

/**
 * @brief Виртуальный класс, описывающий протокол соединения с ФР.
 */
class Protocol
{
public:
    /**
     * @brief Конструктор.
     */
    Protocol();

    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~Protocol();

    /**
     * @brief Метод для оборачивания пакета данными.
     */
    virtual ByteArray proceedData(const ByteArray &b) = 0;

    /**
     * @brief Метод для обработки пакета от устройства.
     * Если пакет не верен - то возвращается пустой массив.
     */
    virtual ByteArray proceedReceivedData(const ByteArray &raw) = 0;

    /**
     * @brief Метод для получения необходимости отвечать
     * устройству после получения данных. Если true - то
     * обращаться за данными к функции Protocol::afterReceivedDataResponse.
     * @return Требуется ли отвечать устройству.
     */
    virtual bool needResponseAfterReceivedData() const = 0;

    /**
     * @brief Метод для получения данных для ответа устройству
     * после получения от него ответа.
     * @return Данные для передачи устройству.
     */
    virtual ByteArray responseAfterReceivedData() = 0;

    /**
     * @brief Метод для получения ответа от устройства, согласно интерфейсу.
     * @param physicalInterface Интерфейс.
     * @param responseBytesExpected Ожидаемое количество байт. Если 0 - количество будет высчитано.
     * @return Данные от устройства.
     */
    virtual ByteArray receiveDataFromInterface(InterfacePtr physicalInterface, uint32_t responseBytesExpected ) = 0;

    /**
     * @brief Метод для подготовки ФР к отправке новых данных.
     * @param physicalInterface Интерфейс.
     */
    virtual void prepareDeviceToWrite(InterfacePtr physicalInterface) = 0;

    /**
     * @brief Метод для проверки наличия соединения с устройством.
     * @param physicalInterface Интерфейс через который будет
     * производиться проверка.
     * @return Присутствует ли соединение
     */
    virtual bool checkConnection(InterfacePtr physicalInterface) = 0;
};


#endif //FRDRVCROSS_PROTOCOL_H
