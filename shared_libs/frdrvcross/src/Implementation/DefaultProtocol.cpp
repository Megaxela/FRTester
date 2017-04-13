//
// Created by megaxela on 14.11.16.
//

#include "Implementation/DefaultProtocol.h"
#include "Tools/Logger.h"

#define ENQ 0x05
#define STX 0x02
#define ACK 0x06
#define NAK 0x15

DefaultProtocol::DefaultProtocol() : Protocol()
{

}

DefaultProtocol::~DefaultProtocol()
{

}

ByteArray DefaultProtocol::wrapData(const ByteArray &b)
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

ByteArray DefaultProtocol::proceedReceivedData(const ByteArray &raw)
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

bool DefaultProtocol::needResponseAfterReceivedData() const
{
    return true;
}

ByteArray DefaultProtocol::responseAfterReceivedData()
{
    ByteArray data(1);
    data.append<uint8_t>(0x06);
    return data;
}

ByteArray DefaultProtocol::receiveDataFromInterface(InterfacePtr physicalInterface, uint32_t /*responseBytesExpected*/)
{
    ByteArray byteArray;

    // Считываем ACK
    ByteArray expectAck = physicalInterface->read(
            1, 60 * 1000 * 1000 // 60 секунд
    );

    if (expectAck.empty())
    {
        Error("Таймаут считывания ACK с ФР.");
        return byteArray;
    }

    if (expectAck[0] == 0xFF)
    {
        Warning("На этапе считывания данных вместо ACK был получен 0xFF. Пробуем еще раз.");
        expectAck = physicalInterface->read(
                1, 50 * 1000 // 5 секунд
        );
    }

    if (expectAck[0] != ACK)
    {
        Error("Получен неожиданный символ. Вместо ACK получен 0x" +
              expectAck.toHex());

        return byteArray;
    }

    // ФР получил команду и готовит ответ.
    ByteArray expectStx = physicalInterface->read(
            1, 2 * 60 * 1000 * 1000 // 2 Минуты
    );

    if (expectStx.empty())
    {
        Error("Таймаут считывания STX с ФР.");
        return byteArray;
    }

    if (expectStx[0] != STX)
    {
        Warning("После ACK получен неожиданный байт. Вместо STX получен 0x" +
                expectStx.toHex());

        Log("Пробуем считать еще один байт.");
        expectStx = physicalInterface->read(
                1, 2 * 1000 * 1000 // 2 секунды
        );

        if (expectStx.empty())
        {
            Error("Больше данных от ККТ не поступало. FAIL.");
            return byteArray;
        }

        if (expectStx[0] != STX)
        {
            Error("И это опять не STX. Ответ: 0x" + expectStx.toHex());
            return byteArray;
        }
    }

    // Считываем длину ответа
    ByteArray dataSize = physicalInterface->read(
            1, 50 * 1000 // 5 Секунд
    );

    if (dataSize.empty())
    {
        Error("Таймаут получения длины данных после получения STX.");
        return byteArray;
    }

    ByteArray data = physicalInterface->read(
            dataSize[0], 50 * 1000 // 5 Секунд
    );

    if (data.empty() && dataSize[0] != 0)
    {
        Error("Не удалось получить данные, хотя количество данных было получено.");
        return byteArray;
    }

    ByteArray checkSum = physicalInterface->read(
            1, 50 * 1000 // 5 секунд
    );

    byteArray.append(expectStx);
    byteArray.append(dataSize);
    byteArray.append(data);
    byteArray.append(checkSum);

    return byteArray;
}

Protocol::Error DefaultProtocol::prepareDeviceToWrite(InterfacePtr physicalInterface)
{
    ByteArray enqArray;
    enqArray.append<uint8_t>(ENQ);

    ByteArray ackArray;
    ackArray.append<uint8_t>(ACK);

    physicalInterface->write(enqArray);

    ByteArray data = physicalInterface->read(1, 1000 * 50); // 50 ms

    while (true)
    {
        if (data.empty())
        {
            Error("Не удалось получить ответ на ENQ.");
            return Error::Timeout;
        }

        // ФР Готов принимать данные
        if (data[0] == NAK)
        {
            return Error::NoError;
        }
            // От ФР остались неподтвержденные данные
            // При чем ACK куда-то потерялся
        else if (data[0] == STX)
        {
            // Считываем данные и подтверждаем их
            ByteArray length = physicalInterface->read(1, 1000 * 50); // 50 ms

            if (length.empty())
            {
                Error("Ошибка при считывании длины оставшихся неподтвержденных данных.");
                return Error::Timeout;
            }

            ByteArray response = physicalInterface->read(length[0], 1000 * 50); // 50 ms

            if (response.empty())
            {
                Error("Ошибка при считывании оставшихся неподтвержденных данных.");
                return Error::Timeout;
            }

            ByteArray checkSum = physicalInterface->read(1, 1000 * 50); // 50 ms

            if (response.empty())
            {
                Error("Ошибка при считывании контрольной суммы.");
                return Error::Timeout;
            }

            physicalInterface->write(ackArray);

            // После этого ФР должен быть готов к работе
            return Error::NoError;
        }

        else if (data[0] == ACK)
        {
            ByteArray stxExp = physicalInterface->read(1, 1000 * 50); // 50 ms

            if (stxExp[0] != STX)
            {
                Critical("Какая-то ахинея исходит от ФР.");
                return Error::UknownBehaviour;
            }

            // Считываем данные и подтверждаем их
            ByteArray length = physicalInterface->read(1, 1000 * 50); // 50 ms

            if (length.empty())
            {
                Critical("Не удалось получить длину данных для получения.");
                return Error::Timeout;
            }

            ByteArray response = physicalInterface->read(length[0], 1000 * 50); // 50 ms

            if (response.empty())
            {
                Critical("Не удалось получить данные для получения.");
                return Error::Timeout;
            }

            ByteArray checkSum = physicalInterface->read(1, 1000 * 50); // 50 ms

            if (checkSum.empty())
            {
                Critical("Не удалось получить контрольную сумму данных для получения.");
                return Error::Timeout;
            }

            physicalInterface->write(ackArray);

            physicalInterface->write(enqArray);

            data = physicalInterface->read(1, 1000 * 50); // 50 ms

            continue;

            // После этого ФР должен быть готов к работе
        }
        else if (data[0] == 0xff) // todo: Странное поведение
        {
            data = physicalInterface->read(1, 1000 * 50); // 50 ms
        }
        else
        {
            Critical("Не известное поведение.");
            return Error::UknownBehaviour;
        }
    }
}

bool DefaultProtocol::checkConnection(InterfacePtr physicalInterface)
{
    prepareDeviceToWrite(physicalInterface);

    ByteArray enqArr(1);
    enqArr.append<uint8_t>(ENQ);

    // Отправляем 05
    physicalInterface->write(enqArr);

    // Ожидаем 15
    ByteArray result = physicalInterface->read(1, 50 * 1000);

    return !result.empty();
}
