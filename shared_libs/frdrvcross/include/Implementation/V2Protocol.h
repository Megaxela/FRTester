//
// Created by megaxela on 05.04.17.
//

#ifndef FRTESTER_V2PROTOCOL_H
#define FRTESTER_V2PROTOCOL_H

#include <Abstract/Protocol.h>

/**
 * @brief Протокол V2.
 */
class V2Protocol : public Protocol
{
public:
    /**
     * @brief Конструктор.
     */
    V2Protocol();

    /**
     * @brief Деструктор.
     */
    ~V2Protocol();

    /**
     * @brief Метод для обработки исходящих пакетов.
     * @param b Данные команды для отправки.
     * @return Обернутые данные, которые готовы к передаче
     * на устройство.
     */
    ByteArray wrapData(const ByteArray &b) override;

    /**
     * @brief Метод для обработки входящих пакетов.
     * @param raw Данные пришедшие с устройства.
     * @return Данные, готовые к обработке.
     */
    ByteArray proceedReceivedData(const ByteArray& raw) override;

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
     * @param inter Интерфейс.
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
     * @brief Метод для проверки наличия соединения.
     * @param physicalInterface Интерфейс.
     * @return Результат наличия соединения.
     */
    bool checkConnection(InterfacePtr physicalInterface) override;
};


#endif //FRTESTER_V2PROTOCOL_H
