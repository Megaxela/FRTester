//
// Created by megaxela on 16.11.16.
//

#ifndef FRDRVCROSS_TESTPROTOCOL_H
#define FRDRVCROSS_TESTPROTOCOL_H


#include "../Tools/ByteArray.h"
#include "../Abstract/PhysicalInterface.h"
#include "../Abstract/Protocol.h"

class TestProtocol : public Protocol
{
public:
    /**
     * @brief Конструктор.
     */
    TestProtocol();

    /**
     * @brief Деструктор.
     */
    ~TestProtocol();

    /**
     * @brief Метод для обработки исходящий пакетов.
     * @param b Данные команды для отправки.
     * @return Обернутые данные, которые к передаче на устройство.
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
    void prepareDeviceToWrite(InterfacePtr physicalInterface) override;

    /**
     * @brief Метод для установки режима пропуска чтения.
     * @param spend Значение.
     */
    void setSpendRead(bool spend);

    /**
     * @brief Метод для получения режима пропуска чтения.
     * @return Режим.
     */
    bool isReadSpent() const;

private:
    bool m_spendRead;
};


#endif //FRDRVCROSS_TESTPROTOCOL_H
