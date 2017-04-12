//
// Created by megaxela on 14.11.16.
//

#ifndef FRDRVCROSS_DEFAULTPROTOCOL_H
#define FRDRVCROSS_DEFAULTPROTOCOL_H


#include "../Abstract/Protocol.h"

/**
 * @brief Стандартный протокол.
 */
class DefaultProtocol : public Protocol
{
public:
    /**
     * @brief Конструктор.
     */
    DefaultProtocol();

    /**
     * @brief Деструктор.
     */
    ~DefaultProtocol();

    /**
     * @brief Метод для обработки исходящий пакетов.
     * @param b Данные команды для отправки.
     * @return Обернутые данные, которые готовы к передаче на устройство.
     */
    ByteArray wrapData(const ByteArray &b) override;

    /**
     * @brief Метод для обработки входящих пакетов.
     * @param raw Данные пришедшие с устройства.
     * @return Данные, готовые к обработке.
     */
    ByteArray proceedReceivedData(const ByteArray &raw) override;

    /**
     * @brief Получение необходимости отправки данных
     * после получения.
     * @return Необходимость отправки.
     */
    bool needResponseAfterReceivedData() const override;

    /**
     * @brief Метод для получения данных для отправки
     * после получения.
     * @return Данные для отправки.
     */
    ByteArray responseAfterReceivedData() override;

    /**
     * @brief Метод для считывания данных с интерфейса.
     * @param inter Интерфейс
     * @param responseBytesExpected Количество ожидаемых данных.
     * @return Считанные данные.
     */
    ByteArray receiveDataFromInterface(InterfacePtr inter, uint32_t responseBytesExpected) override;

    /**
     * @brief Метод для подготовки устройства к записи.
     * @param physicalInterface Интерфейс.
     */
    Error prepareDeviceToWrite(InterfacePtr physicalInterface) override;

    /**
     * @brief Метод для проверки соединения.
     * @param physicalInterface Интерфейс.
     * @return Результат наличия соединения.
     */
    bool checkConnection(InterfacePtr physicalInterface) override;
};


#endif //FRDRVCROSS_DEFAULTPROTOCOL_H
