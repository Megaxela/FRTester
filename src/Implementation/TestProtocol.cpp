//
// Created by megaxela on 16.11.16.
//

#include "Implementation/TestProtocol.h"
#include "Abstract/Protocol.h"
#include "Tools/Logger.h"

#define ENQ 0x05
#define STX 0x02
#define ACK 0x06
#define NAK 0x15

TestProtocol::TestProtocol() :
        Protocol(),
        m_spendRead(false)
{

}

TestProtocol::~TestProtocol()
{

}

ByteArray TestProtocol::proceedData(const ByteArray &b)
{
    ByteArray newData(b.length() + 3);

    // Добавляем STX
    newData.append<uint8_t>(STX);

    // Добавляем длину данных
    newData.append<uint8_t>(static_cast<uint8_t>(b.length()));

    // Добавляем данные
    newData.append(b);

    // Высчитываем контрольную сумму и добавляем ее
    uint8_t checkSum = 0;
    for (uint32_t i = 1; i < newData.length(); ++i)
    {
        checkSum ^= newData[i];
    }

    newData.append<uint8_t>(checkSum);

    return newData;
}

ByteArray TestProtocol::proceedReceivedData(const ByteArray &raw)
{
    // Проверяем наличие данных
    if (raw.empty())
    {
        Error("Получен пустой массив.");
        return ByteArray();
    }

    // Проверяем первый байт массива
    if (raw[0] != STX)
    {
        Error("Первый байт ответа не STX.");
        return ByteArray();
    }

    // Проверяем длину сообщения
    if (raw.length() != static_cast<uint32_t>(raw[1]) + 3)
    {
        Error("Длина сообщения не совпадает с указанным размером.");
        return ByteArray();
    }

    // Считаем контрольную сумму
    uint8_t checkSum = 0;
    for (uint32_t i = 1; i < raw.length() - 1; ++i)
    {
        checkSum ^= raw[i];
    }

    if (raw[raw.length() - 1] != checkSum)
    {
        Error("Контрольная сумма сообщения не совпадает с высчитанной. \n"
                      "    Высчитанная:       " + std::to_string(checkSum) + "\n"
                      "    Указанная в пакете:" + std::to_string(raw[raw.length() - 1]));
        return ByteArray();
    }

    return raw.mid(2, raw.length() - 3);
}

bool TestProtocol::needResponseAfterReceivedData() const
{
    return true;
}

ByteArray TestProtocol::responseAfterReceivedData()
{
    ByteArray data(1);
    data.append<uint8_t>(0x06);
    return data;
}

ByteArray TestProtocol::receiveDataFromInterface(InterfacePtr inter, uint32_t responseBytesExpected)
{
    ByteArray byteArray;

    // Если установлен пропуск чтения
    if (m_spendRead)
    {
        return byteArray;
    }

    // Считываем ACK
    int retry = 3;
    while (retry)
    {
        ByteArray ackResponse = inter->read(
                1, static_cast<uint32_t>(1000 * 1000)
        );

        if (ackResponse.empty())
        {
            Error("ACK ответ от ФР получен не был. Попробуем запросить ENQ.");
            ByteArray arr;
            arr.append<uint8_t>(ENQ);
            inter->write(arr);
            --retry;
            continue;
        }

        if (ackResponse[0] != ACK)
        {
            if (ackResponse[0] == NAK)
            {
                Error("ФР ожидает команду, хотя команда уже была отправлена. Пробуем еще раз (ENQ).");
                ByteArray arr;
                arr.append<uint8_t>(ENQ);
                inter->write(arr);
                --retry;
                continue;
            }
            else if (ackResponse[0] == STX)
            {
                Critical("По какой-то причине был получен STX.");
                return byteArray;
            }
            else
            {
                Error("Ожидался ACK, а был получен другой байт (DEC(" +
                      std::to_string(ackResponse[0]) + ")). Прекращаем обработку ответа.");
                return byteArray;
            }
        }
        else
        {
            // Все, ACK получен.
            break;
        }
    }

    if (!retry)
    {
        Error("Попытки повторно получить ответ от ФР не увенчались "
                      "успехом 3 раза.");
        return byteArray;
    }

    // Считываем ответ от ККТ (используем максимальный таймаут)
    // + 3, потому что STX, длина и LRC. Если количество ожидаемых
    // данных не было получено - то пытаемся вычислить его самостоятельно

    if (responseBytesExpected != 0)
    {
        byteArray = inter->read(
                responseBytesExpected + 3, 2 * 60 * 1000 // 2 минуты
        );

        if (byteArray.empty())
        {
            Error("Пустой ответ от ФР или таймаут.");
        }
        if (byteArray.length() != responseBytesExpected + 3)
        {
            Warning("Получена неожиданный ответ от ФР. Передадим его дальше.");
        }
    }
    else
    {
        auto stxAndSize = inter->read(
                2, 2 * 60 * 1000 // 2 минуты
        );

        if (stxAndSize.empty())
        {
            Error("Не удалось получить ответ от ФР.");
            return byteArray;
        }
        if (stxAndSize.length() != 2)
        {
            Error("Количество данных не 2 байта. Значит полученные данные не содежат STX + Длину.");
            return byteArray;
        }

        auto data = inter->read(
                stxAndSize[1], 100 // todo: Установить более нормальный таймаут
        );

        auto checkSum = inter->read(1, 100);

        byteArray.append(stxAndSize);
        byteArray.append(data);
        byteArray.append(checkSum);

        return byteArray;
    }

    return byteArray;
}

void TestProtocol::prepareDeviceToWrite(InterfacePtr physicalInterface)
{
    ByteArray enqArray;
    enqArray.append<uint8_t>(ENQ);

    ByteArray ackArray;
    ackArray.append<uint8_t>(ACK);

    physicalInterface->write(enqArray);

    ByteArray data = physicalInterface->read(1, 1000 * 50);

    // ФР Готов принимать данные
    if (data[0] == NAK)
    {
        return;
    }
        // От ФР остались неподтвержденные данные
        // При чем ACK куда-то потерялся
    else if (data[0] == STX)
    {
        // Считываем данные и подтверждаем их
        ByteArray length = physicalInterface->read(1, 1000 * 50);

        ByteArray response = physicalInterface->read(length[0], 1000 * 50);

        ByteArray checkSum = physicalInterface->read(1, 1000 * 50);

        physicalInterface->write(ackArray);

        // После этого ФР должен быть готов к работе
        return;
    }
        //
    else if (data[0] == ACK)
    {
        ByteArray stxExp = physicalInterface->read(1, 1000 * 50);

        if (stxExp[0] != STX)
        {
            Critical("Какая-то ахинея исходит от ФР.");
            return;
        }

        // Считываем данные и подтверждаем их
        ByteArray length = physicalInterface->read(1, 1000 * 50);

        ByteArray response = physicalInterface->read(length[0], 1000 * 50);

        ByteArray checkSum = physicalInterface->read(1, 1000 * 50);

        physicalInterface->write(ackArray);

        // После этого ФР должен быть готов к работе
        return;
    }
}

void TestProtocol::setSpendRead(bool spend)
{
    m_spendRead = spend;
}

bool TestProtocol::isReadSpent() const
{
    return m_spendRead;
}
