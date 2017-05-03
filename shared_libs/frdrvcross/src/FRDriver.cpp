//
// Created by megaxela on 11.11.16.
//

#include <map>
#include <Tools/ByteArrayReader.h>
#include "FRDriver.h"
#include "Abstract/PhysicalInterface.h"
#include "Tools/Logger.h"
#include "Tools/Time.h"

FRDriver::FRDriver() :
    m_sendMutex(),
    m_protocol(),
    m_interface(),
    m_commandsDelay(0),
    m_lastErrorCode(ErrorCode::NoError),
    m_lastReceivedCashierNumber(0)
{

}

FRDriver::~FRDriver()
{
    std::unique_lock<std::mutex> lock(m_sendMutex);
}

void FRDriver::setInterface(InterfacePtr inter)
{
    m_interface = inter;
}

InterfacePtr FRDriver::physicalInterface() const
{
    return m_interface;
}

void FRDriver::setProtocol(ProtocolPtr protocol)
{
    m_protocol = protocol;
}

ProtocolPtr FRDriver::protocol() const
{
    return m_protocol;
}

ByteArray FRDriver::sendRaw(const ByteArray &data)
{
    if (!isAdaptorsReady())
    {
        Critical("Попытка отправить данные без установленных адаптеров. Выкидываем исключение.");
        throw DriverException("Адаптеры для драйвера не были установлены.");
    }

    auto error = m_protocol->prepareDeviceToWrite(m_interface);
    if (error != Protocol::Error::NoError)
    {
        // Что-то пошло не по плану
        switch (error)
        {
        case Protocol::Timeout:
            m_lastErrorCode = ErrorCode::Timeout;
            break;
        case Protocol::UknownBehaviour:
            m_lastErrorCode = ErrorCode::Unknown;
            break;
        case Protocol::NoError:break;
        }

        return ByteArray();
    }

    // Оборачиваем данные
    auto formedData = m_protocol->wrapData(data);

    // Отправляем на ФР
    auto sent = 0;

    auto sendTime = Time::timeFunction<std::chrono::microseconds>(
            [&]()
            {
                sent = m_interface->write(formedData);
            }
    );

    ExcessLog("Время отправки заняло: " + std::to_string(sendTime / 1000.0) + " миллисекунд.");

    // Проверяем количество отправленных данных
    if (static_cast<uint32_t>(sent) < formedData.length())
    {
        Warning("На фискальный регистратор были отправлены не все данные. (" +
                std::to_string(sent) + "/" + std::to_string(formedData.length()));
    }

    // Вне зависимости от количества отправляемых данные пытаемся получить ответ
    auto receivedData = m_protocol->receiveDataFromInterface(m_interface, 0);

    if (receivedData.empty())
    {
        Error("От ФР не было получено данных.");
        return receivedData;
    }

    receivedData = m_protocol->proceedReceivedData(receivedData);

    if (m_protocol->needResponseAfterReceivedData())
    {
        m_interface->write(
                m_protocol->responseAfterReceivedData()
        );
    }

    return receivedData;
}

bool FRDriver::isAdaptorsReady() const
{
    return m_interface != nullptr &&
            m_protocol != nullptr;
}

ByteArray FRDriver::sendCommand(const FRDriver::Command &c, const ByteArray &arguments, bool responseHasCashier)
{
    ByteArray packedCommand;

    // Вычисление размера команды в битах.
    uint16_t commandSize = SystemTools::getNumberSize<int>((int) c);

    // В зависимост от размера команды добавляем ее в массив байт.
    if (commandSize > 8)
    {
        packedCommand.append<uint16_t>(
                static_cast<uint16_t>(c),
                ByteArray::ByteOrder_BigEndian
        );
    }
    else
    {
        packedCommand.append<uint8_t>(
                static_cast<uint8_t>(c)
        );
    }

    packedCommand.append(arguments);

    auto response = sendRaw(packedCommand);

    if (!response.empty())
    {
        proceedResponse(response, responseHasCashier);
    }

    return response;
}

void FRDriver::proceedResponse(const ByteArray &data, bool cashier)
{
    if (data.length() == 0)
    {
        Error("Попытка обработать ответ неверной длины. Меняю последнюю ошибку на Unknown.");
        m_lastErrorCode = ErrorCode::Unknown;
        return;
    }

    uint8_t commandSize = (uint8_t) (data[0] == 0xFF ? 2 : 1);
    if (data.length() < static_cast<uint32_t>(commandSize + 1))
    {
        Error("Попытка обработать ответ неверной длины. Меняю последнюю ошибку на Unknown.");
        m_lastErrorCode = ErrorCode::Unknown;
        return;
    }

    m_lastErrorCode = static_cast<ErrorCode>(data[commandSize]);

    Log("Получена ошибка: #" +
        std::to_string(data[commandSize]) +
        " \"" +
        FRDriver::Converters::errorToString(data[commandSize]) +
        "\" из ответа " +
        data.toHex()
    );

    if (cashier)
    {
        if (data.length() > 2)
        {
            m_lastReceivedCashierNumber = data.read<uint8_t>(commandSize + 1);
        }
        else
        {
            Error("Не удалось получить номер касира, поскольку программа выполнилась с ошибкой.");
        }
    }
}

FRDriver::ErrorCode FRDriver::getLastError() const
{
    return m_lastErrorCode;
}

bool FRDriver::beep(uint32_t password)
{
    ByteArray arguments;

    arguments.append<uint32_t>(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::Beep, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

bool FRDriver::sell(uint32_t password,
                    uint64_t count,
                    uint64_t price,
                    uint8_t department,
                    uint8_t firstTax,
                    uint8_t secondTax,
                    uint8_t thirdTax,
                    uint8_t fourthTax,
                    const std::string &good)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(count, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(price, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append(department);
    arguments.append(firstTax);
    arguments.append(secondTax);
    arguments.append(thirdTax);
    arguments.append(fourthTax);
    arguments.append((uint8_t*) good.c_str(),
                     good.length());

    if (good.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, 40 - good.length());
    }

    ByteArray data = sendCommand(Command::Sell, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}



bool FRDriver::returnSell(uint32_t password,
                          uint64_t count,
                          uint64_t price,
                          uint8_t department,
                          uint8_t firstTax,
                          uint8_t secondTax,
                          uint8_t thirdTax,
                          uint8_t fourthTax,
                          const std::string &good)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(count, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(price, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append(department);
    arguments.append(firstTax);
    arguments.append(secondTax);
    arguments.append(thirdTax);
    arguments.append(fourthTax);
    arguments.append((uint8_t*) good.c_str(),
                     good.length());

    if (good.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, 40 - good.length());
    }

    ByteArray data = sendCommand(Command::ReturnSell, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

bool FRDriver::returnBuy(uint32_t password,
                         uint64_t count,
                         uint64_t price,
                         uint8_t department,
                         uint8_t firstTax,
                         uint8_t secondTax,
                         uint8_t thirdTax,
                         uint8_t fourthTax,
                         const std::string &good)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(count, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(price, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append(department);
    arguments.append(firstTax);
    arguments.append(secondTax);
    arguments.append(thirdTax);
    arguments.append(fourthTax);
    arguments.append((uint8_t*) good.c_str(),
                     good.length());

    if (good.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, 40 - good.length());
    }

    ByteArray data = sendCommand(Command::ReturnBuy, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

bool FRDriver::buy(uint32_t password,
                   uint64_t count,
                   uint64_t price,
                   uint8_t department,
                   uint8_t firstTax,
                   uint8_t secondTax,
                   uint8_t thirdTax,
                   uint8_t fourthTax,
                   const std::string &good)
{
    ByteArray arguments;

    arguments.append    (password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(count, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(price, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append    (department);
    arguments.append    (firstTax);
    arguments.append    (secondTax);
    arguments.append    (thirdTax);
    arguments.append    (fourthTax);
    arguments.append    ((uint8_t*) good.c_str(),
                         static_cast<uint32_t>(good.length()));

    if (good.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, static_cast<uint32_t>(40 - good.length()));
    }

    ByteArray data = sendCommand(Command::Buy, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}



bool FRDriver::openShift(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::OpenShift, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

FRDriver::ShortState FRDriver::shortStateRequest(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ShortStateRequest, arguments, false);

    ShortState result = FRDriver::ShortState();

    if (m_lastErrorCode != ErrorCode::NoError)
    {
        return result;
    }

    result.posFlags                     = data.read<uint16_t>(3 , ByteArray::ByteOrder_LittleEndian);
    result.posMode                      = data.read<uint8_t >(5 );
    result.posSubMode                   = data.read<uint8_t >(6 );
    result.checkNumberOfActions         = data.read<uint8_t >(7 );
    result.batteryVoltage               = data.read<uint8_t >(8 );
    result.powerSupplyVoltage           = data.read<uint8_t >(9 );
    result.checkNumberOfActionsHigher   = data.read<uint8_t >(12);

    return result;
}

bool FRDriver::shiftCloseReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ShiftCloseReport, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

uint8_t FRDriver::getLastReceivedCashierNumber() const
{
    return m_lastReceivedCashierNumber;
}

FRDriver::CheckResult
FRDriver::closeCheck(uint32_t password,
                     uint64_t cashPaySum,
                     uint64_t type2PaySum,
                     uint64_t type3PaySum,
                     uint64_t type4PaySum,
                     uint16_t discount,
                     uint8_t tax1,
                     uint8_t tax2,
                     uint8_t tax3,
                     uint8_t tax4,
                     const std::string &textToPrint)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(cashPaySum,  5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type2PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type3PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type4PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append(discount, ByteArray::ByteOrder_LittleEndian);
    arguments.append(tax1);
    arguments.append(tax2);
    arguments.append(tax3);
    arguments.append(tax4);
    arguments.append((uint8_t*) textToPrint.c_str(),
                     textToPrint.length());

    if (textToPrint.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, 40 - textToPrint.length());
    }

    ByteArray data = sendCommand(Command::CloseCheck, arguments, false);

    CheckResult result = CheckResult();

    if (getLastError() != ErrorCode::NoError)
    {
        return result;
    }

    result.change = data.readPart(3, 5, ByteArray::ByteOrder_LittleEndian);

    auto str = data.mid(7, data.length() - 7);
    str.append<uint8_t>(0x00);

    result.url = std::string((char*) str.data());

    return result;
}

uint64_t
FRDriver::closeCheckExtended(uint32_t password,
                             uint64_t cashPaySum,
                             uint64_t type2PaySum,
                             uint64_t type3PaySum,
                             uint64_t type4PaySum,
                             uint64_t type5PaySum,
                             uint64_t type6PaySum,
                             uint64_t type7PaySum,
                             uint64_t type8PaySum,
                             uint64_t type9PaySum,
                             uint64_t type10PaySum,
                             uint64_t type11PaySum,
                             uint64_t type12PaySum,
                             uint64_t type13PaySum,
                             uint64_t type14PaySum,
                             uint64_t type15PaySum,
                             uint64_t type16PaySum,
                             uint16_t discount,
                             uint8_t tax1,
                             uint8_t tax2,
                             uint8_t tax3,
                             uint8_t tax4,
                             const std::string &textToPrint)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(cashPaySum,  5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type2PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type3PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type4PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type5PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type6PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type7PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type8PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type9PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type10PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type11PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type12PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type13PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type14PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type15PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(type16PaySum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append(discount, ByteArray::ByteOrder_LittleEndian);
    arguments.append(tax1);
    arguments.append(tax2);
    arguments.append(tax3);
    arguments.append(tax4);
    arguments.append((uint8_t*) textToPrint.c_str(),
                     textToPrint.length());

    if (textToPrint.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, 40 - textToPrint.length());
    }

    ByteArray data = sendCommand(Command::CloseCheckExtended, arguments, false);

    if (getLastError() != ErrorCode::NoError)
    {
        return 0;
    }

    ByteArrayReader reader(data);

    reader.seek(2);

    return reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
}

bool FRDriver::resumePrinting(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ContinuePrint, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

FRDriver::FullState FRDriver::fullStateRequest(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::FullStateRequest, arguments, false);

    FullState state = FRDriver::FullState();

    if (m_lastErrorCode != ErrorCode::NoError)
    {
        return state;
    }

    ByteArrayReader reader(data);
    reader.seek(3);

    state.firmwareVersion.majorVersion             = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareVersion.minorVersion             = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareBuild                     = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    state.firmwareDate.day                  = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareDate.month                = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareDate.year                 = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.numberInHall                      = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.currentDocumentPassthrougNumber   = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    state.posFlags                          = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    state.posMode                           = (uint8_t) (reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian) & 15); //0b1111 C++14
    state.posSubMode                        = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.posPort                           = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    reader.move(7); // Зарезервировано
    state.date.day                          = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.date.month                        = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.date.year                         = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.time.hour                         = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.time.minute                       = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.time.second                       = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    reader.move(1); // Зарезервировано
    state.factoryNumberLower                = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);
    state.lastClosedShiftNumber             = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    reader.move(2); // Зарезервировано
    state.numberOfReRegistration            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    state.inn[0]                            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    state.inn[1]                            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    state.inn[2]                            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    state.inn[3]                            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    state.inn[4]                            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    state.inn[5]                            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
//    reader.move(2);
//    state.factoryNumberUpper                = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);

    return state;
}

bool FRDriver::cancelCheck(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::CancelCheck, arguments, false);

    return m_lastErrorCode == ErrorCode::NoError;
}

bool FRDriver::checkConnection()
{
    if (!isAdaptorsReady())
    {
        Error("Протокол или интерфейс на определены.");
        return false;
    }

    bool result = m_protocol->checkConnection(m_interface);

    m_lastErrorCode = ErrorCode::NoError;

    return result;
}

uint16_t FRDriver::operatingRegisterRequest(uint32_t password, uint8_t registerNumber)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(registerNumber, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::OperatingRegisterRequest, arguments, false);

    if (data.size() < 5)
    {
        return 0;
    }

    return data.read<uint16_t>(3, ByteArray::ByteOrder_LittleEndian);
}

FRDriver::ExchangeConfiguration FRDriver::readExchangeConfiguration(uint32_t sysAdmPassword, uint8_t portNumber)
{
    ByteArray arguments;

    arguments.append(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(portNumber,     ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ReadExchangeConfiguration, arguments, false);

    ExchangeConfiguration configuration;

    configuration.baudRateCode = data.read<uint8_t>(2);
    configuration.byteTimeout = data.read<uint8_t>(3);

    return configuration;
}

bool FRDriver::technologicalReset()
{
    ByteArray data = sendCommand(Command::TechReset, ByteArray(), false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::standardStringPrint(uint32_t password,
                                   uint8_t flags,
                                   const std::string &print)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(flags);
    arguments.append((uint8_t*) print.c_str(),
                     (uint32_t) print.length());

    if (print.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, (uint32_t) (40 - print.length()));
    }

    auto data = sendCommand(Command::StandardStringPrint, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

uint16_t FRDriver::documentHeaderPrint(uint32_t password,
                                       const std::string &document,
                                       uint16_t documentNumber)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append((uint8_t*) document.c_str(),
                     (uint32_t) document.length());

    if (document.length() < 30)
    {
        arguments.appendMultiple<uint8_t>(0x00, (uint32_t) (30 - document.length()));
    }

    arguments.append(documentNumber, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::DocumentHeaderPrint, arguments, true);

    if (data.length() != 5)
    {
        return 0;
    }

    return data.read<uint16_t>(3, ByteArray::ByteOrder_LittleEndian);
}

uint64_t FRDriver::currencyRegisterRequest(uint32_t password, uint8_t registerNumber)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(registerNumber);

    auto data = sendCommand(Command::CurrencyRegisterRequest, arguments, true);

    if (data.length() != 9)
    {
        return 0;
    }

    return data.readPart(3, 6, ByteArray::ByteOrder_LittleEndian);
}

bool FRDriver::writeTable(uint32_t sysPassword,
                          uint8_t tableNumber,
                          uint16_t row,
                          uint8_t field,
                          const std::string &value)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(tableNumber);
    arguments.append(row, ByteArray::ByteOrder_LittleEndian);
    arguments.append(field);
    arguments.append((uint8_t*) value.c_str(),
                     (uint32_t) value.length());

    if (value.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, (uint32_t) (40 - value.length()));
    }

    auto data = sendCommand(Command::WriteTable, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::timeProgramming(uint32_t sysPassword,
                               uint8_t h,
                               uint8_t m,
                               uint8_t s)
{
    ByteArray arguments;
    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(h);
    arguments.append(m);
    arguments.append(s);

    auto data = sendCommand(Command::TimeProgramming, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::dateProgramming(uint32_t sysPassword,
                               uint8_t day,
                               uint8_t month,
                               uint8_t year)
{
    ByteArray arguments;
    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(day);
    arguments.append(month);
    arguments.append(year);

    auto data = sendCommand(Command::DateProgramming, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::dateConfirm(uint32_t sysPassword,
                           uint8_t day,
                           uint8_t month,
                           uint8_t year)
{
    ByteArray arguments;
    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(day);
    arguments.append(month);
    arguments.append(year);

    auto data = sendCommand(Command::DateConfirm, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::tableValuesInit(uint32_t sysPassword)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::TableValuesInit, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::cutCheck(uint32_t sysPassword, uint8_t cutType)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(cutType);

    auto data = sendCommand(Command::CutCheck, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::FontConfiguration FRDriver::readFontConfiguration(uint32_t sysPassword,
                                                            uint8_t fontNumber)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(fontNumber);

    auto data = sendCommand(Command::ReadFontConfiguration, arguments, false);

    FontConfiguration configuration;

    if (data.length() != 7)
    {
        return configuration;
    }

    // todo: Заменить на ByteArrayReader
    configuration.printAreaWidthPixels      = data.read<uint16_t>(2, ByteArray::ByteOrder_LittleEndian);
    configuration.symbolWidthWithInterval   = data.read<uint8_t>(4);
    configuration.symbolHeightWithInterval  = data.read<uint8_t>(5);
    configuration.numberOfFonts             = data.read<uint8_t>(6);

    return configuration;
}

bool FRDriver::totalExtinction(uint32_t sysPassword)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::TotalExtinction, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::scrolling(uint32_t password, uint8_t flags, uint8_t numberOfLines)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(flags);
    arguments.append(numberOfLines);

    auto data = sendCommand(Command::Scrolling, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::TableStructure FRDriver::tableStructureRequest(uint32_t sysPassword,
                                                         uint8_t tableNumber)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(tableNumber);

    auto data = sendCommand(Command::TableStructureRequest, arguments, false);

    TableStructure structure = FRDriver::TableStructure();

    if (data.length() != 45)
    {
        return structure;
    }

    ByteArrayReader reader(data);
    reader.move(2);

    structure.name            = reader.readString(40);
    structure.numberOfLines   = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    structure.numberOfFields  = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    return structure;
}

FRDriver::FieldStructure FRDriver::fieldStructureRequest(uint32_t sysPassword, uint8_t table, uint8_t field)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(table);
    arguments.append(field);

    auto data = sendCommand(Command::FieldStructureRequest, arguments, false);

    FieldStructure structure = FRDriver::FieldStructure();

    if (data.length() < 44)
    {
        return structure;
    }

    // todo: Заменить на ByteArrayReader
    structure.name          = std::string((const char*) data.data() + 2, 40);
    structure.fieldType     = data.read<uint8_t>(42);
    structure.numberOfBytes = data.read<uint8_t>(43);
//    structure.minValue      = std::string((const char*) data.data() + 44, structure.numberOfBytes);
//    structure.minValue      = std::string((const char*) data.data() + 44 + structure.numberOfBytes, structure.numberOfBytes);

    if (structure.fieldType == FieldStructure::FieldType::Bin)
    {
        structure.minValue      = data.readPart(
                44,
                structure.numberOfBytes,
                ByteArray::ByteOrder_LittleEndian
        );
        structure.minValue      = data.readPart(
                44 + structure.numberOfBytes,
                structure.numberOfBytes,
                ByteArray::ByteOrder_LittleEndian
        );
    }
//     todo: Удостовериться в правильности типа данных
    return structure;
}

bool FRDriver::fontStringPrint(uint32_t password,
                               uint8_t flags,
                               uint8_t fontNumber,
                               const std::string &string)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(flags);
    arguments.append(fontNumber);
    arguments.append((uint8_t*) string.c_str(),
                     (uint32_t) string.length());

    if (string.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, (uint32_t) (40 - string.length()));
    }

    auto data = sendCommand(Command::FontStringPrint, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::shiftReportWithoutExtinction(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::ShiftReportNoExtinction, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::sectionsReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::SectionsReport, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::taxesReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::TaxesReport, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::cashierReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::CashierReport, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

uint16_t FRDriver::payin(uint32_t password, uint64_t sum)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(sum, 5, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PayIn, arguments, true);

    if (data.length() != 5)
    {
        return 0;
    }

    return data.read<uint16_t>(3, ByteArray::ByteOrder_LittleEndian);
}

uint16_t FRDriver::payout(uint32_t password, uint64_t sum)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(sum, 5, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PayOut, arguments, true);

    if (data.length() != 5)
    {
        return 0;
    }

    return data.read<uint16_t>(3, ByteArray::ByteOrder_LittleEndian);
}

bool FRDriver::printCliches(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PrintCliches, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::printDocumentEnd(uint32_t password, uint8_t printAds)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(printAds);

    auto data = sendCommand(Command::DocumentEndPrint, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::printAds(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PrintAds, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::enterFactoryNumber(uint32_t password, uint32_t factoryNumber)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(factoryNumber, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::EnterFactoryNumber, arguments, false);

    return getLastError() != ErrorCode::NoError;
}

bool FRDriver::enterFactoryNumber(uint32_t factoryNumber)
{
    return enterFactoryNumber(0, factoryNumber);
}

FRDriver::InformExchangeStatus FRDriver::getInformationExchangeStatus(uint32_t sysAdmPassword)
{
    ByteArray arguments;

    arguments.append(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::GetInformationExchangeStatus, arguments, false);

    InformExchangeStatus status = InformExchangeStatus();

    if (data.length() != 16)
    {
        return status;
    }

    ByteArrayReader reader(data);
    reader.move(3);

    status.status               = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    status.readStatus           = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    status.messagesForOfd       = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    status.firstDocumentNumber  = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);
    status.date.year            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    status.date.month           = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    status.date.day             = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    status.time.hour            = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    status.time.minute          = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);

    return status;
}

std::string FRDriver::readTableStr(uint32_t password,
                                   uint8_t table,
                                   uint16_t row,
                                   uint8_t field)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(table,    ByteArray::ByteOrder_LittleEndian);
    arguments.append(row,      ByteArray::ByteOrder_LittleEndian);
    arguments.append(field,    ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ReadTable, arguments, true);

    for (uint32_t byteIndex = 2; byteIndex < data.length(); ++byteIndex)
    {
        if (data[byteIndex] == 0x00)
        {
            return std::string((const char*) (data.data() + 2), byteIndex - 2);
        }
    }

    return std::string((const char*) (data.data() + 2), data.length() - 2);
}

uint64_t FRDriver::readTableBin(uint32_t password,
                                uint8_t table,
                                uint16_t row,
                                uint8_t field)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(table,    ByteArray::ByteOrder_LittleEndian);
    arguments.append(row,      ByteArray::ByteOrder_LittleEndian);
    arguments.append(field,    ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ReadTable, arguments, true);

    return data.readPart(2, (uint8_t) (data.length() - 2), ByteArray::ByteOrder_LittleEndian );
}

bool FRDriver::writeTable(uint32_t sysPassword,
                          uint8_t tableNumber,
                          uint16_t row,
                          uint8_t field,
                          uint64_t value,
                          uint8_t valSize)
{
    ByteArray arguments;

    arguments.append(sysPassword,        ByteArray::ByteOrder_LittleEndian);
    arguments.append(tableNumber,        ByteArray::ByteOrder_LittleEndian);
    arguments.append(row,                ByteArray::ByteOrder_LittleEndian);
    arguments.append(field,              ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(value, valSize, ByteArray::ByteOrder_LittleEndian);

    Log("Arguments: " + arguments.toHex());

    auto data = sendCommand(Command::WriteTable, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::NonZeroSums FRDriver::getNonZeroSums()
{
    ByteArray arguments;

    arguments.append((const uint8_t*) "\xF4\x00\x00\x00\x00", 5);

    auto data = sendCommand(Command::NonZeroSums, arguments, false);

    NonZeroSums sums = FRDriver::NonZeroSums();

    if (data.length() != 34)
    {
        return sums;
    }

    ByteArrayReader reader(data);
    reader.seek(2);

    sums.incomingSum          = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);
    sums.consumptionSum       = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);
    sums.returnIncomingSum    = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);
    sums.returnConsumptionSum = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);

    return sums;
}

FRDriver::PingResult FRDriver::ping(const std::string &uri)
{
    ByteArray arguments;

    arguments.append((const uint8_t*) uri.c_str(), (uint32_t) uri.size());

    auto data = sendCommand(Command::Ping, arguments, false);

    PingResult result = FRDriver::PingResult();

    if (getLastError() != ErrorCode::NoError)
    {
        return result;
    }

    ByteArrayReader reader(data);
    reader.seek(2);

    result.success = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian) == 0;

    if (!result.success)
    {
        return result;
    }

    result.time = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);

    return result;
}

bool FRDriver::reboot()
{
    ByteArray arguments;

    arguments.append((const uint8_t*) "\x00\x00\x00\x00", 4);

    auto data = sendCommand(Command::Reboot, arguments, false);

    return true; // todo: IDK
}

uint64_t FRDriver::checkResult(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::CheckResult, arguments, true);

    if (getLastError() != ErrorCode::NoError)
    {
        return 0;
    }

    ByteArrayReader reader(data);

    reader.seek(3);

    return reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
}

bool FRDriver::openNonFiscalDocument(uint32_t pwd)
{
    ByteArray arguments;

    arguments.append(pwd, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::OpenNonFiscalDocument, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::closeNonFiscalDocument(uint32_t pwd)
{
    ByteArray arguments;

    arguments.append(pwd, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::CloseNonFiscalDocument, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::printBarcode(uint32_t pwd, uint64_t value)
{
    ByteArray arguments;

    arguments.append(pwd, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(value, 5, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PrintEANBarcode, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::sendTag(uint32_t pwd, uint16_t tag, const std::string &str)
{
    ByteArray arguments;

    arguments.append(pwd, ByteArray::ByteOrder_LittleEndian);
    arguments.append(tag, ByteArray::ByteOrder_LittleEndian);
    arguments.append((uint16_t) str.size(), ByteArray::ByteOrder_LittleEndian);
    arguments.append((uint8_t*) str.c_str(), (uint32_t) str.size());

    auto data = sendCommand(Command::SendTag, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::BarcodeData
FRDriver::printMultidimensionalBarcode(uint32_t password,
                                       FRDriver::BarcodeType barcodeType,
                                       uint16_t dataLength,
                                       uint8_t startBlock,
                                       uint8_t firstParam,
                                       uint8_t secondParam,
                                       uint8_t thirdParam,
                                       uint8_t fourthParam,
                                       uint8_t fifthParam,
                                       uint8_t align)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append((uint8_t) barcodeType, ByteArray::ByteOrder_LittleEndian);
    arguments.append(dataLength, ByteArray::ByteOrder_LittleEndian);
    arguments.append(startBlock);
    arguments.append(firstParam);
    arguments.append(secondParam);
    arguments.append(thirdParam);
    arguments.append(fourthParam);
    arguments.append(fifthParam);
    arguments.append(align);

    auto data = sendCommand(Command::MultidimensionalBarcode, arguments, false);

    BarcodeData barcodeData = FRDriver::BarcodeData();

    if (getLastError() != ErrorCode::NoError)
    {
        return barcodeData;
    }

    if (data.length() == 3)
    {
        return barcodeData;
    }

    ByteArrayReader reader(data);

    reader.seek(3);

    barcodeData.firstParam = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    barcodeData.secondParam = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    barcodeData.thirdParam = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    barcodeData.fourthParam = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    barcodeData.fifthParam = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    barcodeData.barcodeWidth = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    barcodeData.barcodeHeight = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);

    return barcodeData;
}

uint64_t FRDriver::operationV2(uint32_t password,
                               FRDriver::OperationType operation,
                               uint64_t count,
                               uint64_t price,
                               uint64_t taxValue,
                               uint8_t taxRate,
                               uint8_t department,
                               uint8_t calculationMethod,
                               uint8_t calculationSubject,
                               const std::string &goodName)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append((uint8_t) operation);
    arguments.appendPart(count,    6, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(price,    5, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(taxValue, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append(taxRate);
    arguments.append(department);
    arguments.append(calculationMethod);
    arguments.append(calculationSubject);

    arguments.append(
            reinterpret_cast<const uint8_t *>(goodName.c_str()),
            static_cast<uint32_t>(goodName.size())
    );
    arguments.appendMultiple(0x00, static_cast<uint32_t>(128 - goodName.size()));

    auto response = sendCommand(Command::OperationV2, arguments, false);

    if (getLastError() != FRDriver::ErrorCode::NoError)
    {
        return 0;
    }

    ByteArrayReader reader(response);

    reader.seek(1);

    return reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
}

bool FRDriver::changeFirmware(const std::string &newNumber, const std::string &oldLicense)
{
    if (newNumber.size() != 16)
    {
        ErrorStream()
                << "Не верный размер нового номера "
                << newNumber.size()
                << " != "
                << 16
                << std::endl;
        return false;
    }

    if (oldLicense.size() != 8)
    {
        ErrorStream()
                << "Не верный размер старой лицензии "
                << oldLicense.size()
                << " != "
                << 8
                << std::endl;
        return false;
    }

    ByteArray arguments;

    arguments.append(
            (const uint8_t*) newNumber.c_str(),
            static_cast<uint32_t>(newNumber.size())
    );

    arguments.append(
            (const uint8_t*) oldLicense.c_str(),
            static_cast<uint32_t>(oldLicense.size())
    );

    auto result = sendCommand(Command::ChangeSerialNumber, arguments, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::loadData(uint32_t password,
                        uint8_t dataType,
                        uint8_t blockNumber,
                        ByteArray data)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(dataType);
    arguments.append(blockNumber);
    arguments.append(data);

    if (data.length() < 64)
    {
        arguments.appendMultiple<uint8_t>(0, 64 - data.length(), ByteArray::ByteOrder_LittleEndian);
    }
    else if (data.length() > 64)
    {
        Critical(
            "Массив с данными слишком велик! " +
            std::to_string(data.length()) +
            " > 64"
        );
    }

    sendCommand(Command::LoadData, arguments, true);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::FNStatus FRDriver::getFNStatus(uint32_t sysAdmPassword)
{
    ByteArray arguments;

    arguments.append(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);

    auto response = sendCommand(Command::FNStatusRequest, arguments, false);

    FRDriver::FNStatus status = FRDriver::FNStatus();

    if (getLastError() != ErrorCode::NoError)
    {
        return status;
    }

    ByteArrayReader reader(response);

    reader.seek(3);

    uint8_t lifeCycleState = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    status.settedUp = static_cast<bool>(lifeCycleState & 1);                  // 0b00000001
    status.fiscalModeOpened = static_cast<bool>(lifeCycleState & 2);          // 0b00000010
    status.fiscalModeClosed = static_cast<bool>(lifeCycleState & 4);          // 0b00000100
    status.fiscalDataSendingFinished = static_cast<bool>(lifeCycleState & 8); // 0b00001000
    status.currentDocument = static_cast<FNStatus::Document>(
            reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian)
    );
    status.documentData = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    status.shiftOpened = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    status.warningsFlags = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    status.date.year = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    status.date.month = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    status.date.day = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    status.time.hour = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    status.time.minute = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    status.fnNumber = reader.readString(16);
    status.lastDocumentNumber = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);

    return status;
}

std::string FRDriver::getFNNumber(uint32_t sysAdmPassword)
{
    ByteArray arguments;
    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);

    auto response = sendCommand(Command::FNNumberRequest, arguments, false);

    if (getLastError() != FRDriver::ErrorCode::NoError)
    {
        return std::string();
    }

    ByteArrayReader reader(response);
    reader.seek(3);

    return reader.readString(16);
}

FRDriver::FNVersion FRDriver::getFNVersion(uint32_t sysAdmPassword)
{
    ByteArray arguments;
    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);

    auto response = sendCommand(Command::FNVersionRequest, arguments, false);

    FRDriver::FNVersion version = FRDriver::FNVersion();

    if (getLastError() != FRDriver::ErrorCode::NoError)
    {
        return version;
    }

    ByteArrayReader reader(response);
    reader.seek(3);

    version.firmwareVersion = reader.readString(16);
    version.type = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    return version;
}

bool FRDriver::beginPOSRegistration(uint32_t sysAdmPassword)
{
    ByteArray arguments;
    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);

    auto response = sendCommand(Command::FNStartRegistrationReport, arguments, false);

    return getLastError() == FRDriver::ErrorCode::NoError;
}

FRDriver::POSRegistrationReport FRDriver::formPOSRegistrationReport(uint32_t sysAdmPassword,
                                                                    const std::string &inn,
                                                                    const std::string &posRegistrationNumber,
                                                                    uint8_t taxMode,
                                                                    uint8_t workMode)
{
    assert(inn.size() <= 12);
    assert(posRegistrationNumber.size() <= 20);

    ByteArray arguments;
    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append((const uint8_t *) inn.c_str(), static_cast<uint32_t>(inn.size()));
    arguments.appendMultiple<uint8_t>(0, static_cast<uint32_t>(12 - inn.size()));
    arguments.append((const uint8_t *) posRegistrationNumber.c_str(), static_cast<uint32_t>(posRegistrationNumber.size()));
    arguments.appendMultiple<uint8_t>(0, static_cast<uint32_t>(20 - posRegistrationNumber.size()));
    arguments.append<uint8_t>(taxMode);
    arguments.append<uint8_t>(workMode);

    auto response = sendCommand(Command::FNFormRegistrationReport, arguments, false);

    POSRegistrationReport registrationReport = POSRegistrationReport();

    if (getLastError() != ErrorCode::NoError)
    {
        return registrationReport;
    }

    ByteArrayReader reader(response);
    reader.seek(3);

    registrationReport.fdNumber = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);
    registrationReport.fiscalSign = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);

    return registrationReport;
}

FRDriver::POSRegistrationReport FRDriver::formPOSReRegistrationReport(uint32_t sysAdmPassword, uint8_t reason)
{
    ByteArray arguments;

    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append<uint8_t>(reason);

    auto response = sendCommand(Command::FormReRegistrationReport, arguments, false);

    POSRegistrationReport registrationReport = POSRegistrationReport();

    if (getLastError() != ErrorCode::NoError)
    {
        return registrationReport;
    }

    ByteArrayReader reader(response);
    reader.seek(3);

    registrationReport.fdNumber = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);
    registrationReport.fiscalSign = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);

    return registrationReport;
}

bool FRDriver::resetFNState(uint32_t sysAdmPassword, uint8_t code)
{
    ByteArray arguments;

    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append<uint8_t>(code);

    auto response = sendCommand(Command::ResetFN, arguments, false);

    return getLastError() != ErrorCode::NoError;
}

FRDriver::DeviceType FRDriver::getDeviceType()
{
    auto response = sendCommand(Command::RequestDeviceType, ByteArray(), false);

    FRDriver::DeviceType deviceType = FRDriver::DeviceType();

    if (getLastError() != ErrorCode::NoError)
    {
        return deviceType;
    }

    ByteArrayReader reader(response);
    reader.seek(2);

    deviceType.deviceType = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    deviceType.deviceSubType = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    deviceType.protocolVersion = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    deviceType.protocolSubVersion = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    deviceType.deviceModel = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    deviceType.language = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    deviceType.deviceName = reader.readString(response.size() - 8);

    return deviceType;
}

bool FRDriver::printLine(uint32_t password, const ByteArray &data)
{
    ByteArray arguments;
    arguments.append<uint32_t>(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append<uint16_t>(data.size(), ByteArray::ByteOrder_LittleEndian);
    arguments.append(data);

    auto answer = sendCommand(Command::PrintLine, arguments, false);

    return getLastError() == FRDriver::ErrorCode::NoError;
}

FRDriver::FNDocument FRDriver::findDocument(uint32_t sysAdmPassword, uint32_t documentNumber)
{
    ByteArray arguments;

    arguments.append<uint32_t>(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append<uint32_t>(documentNumber, ByteArray::ByteOrder_LittleEndian);

    auto response = sendCommand(Command::FindFiscalDocument, arguments, false);

    FRDriver::FNDocument document = FRDriver::FNDocument();

    if (getLastError() != ErrorCode::NoError)
    {
        return document;
    }

    ByteArrayReader reader(response);

    reader.seek(3);

    document.type = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);
    document.receiptReceived = reader.read<uint8_t>(ByteArray::ByteOrder_LittleEndian);

    //todo: Добавить сюда получение TLV данных.
    return document;
}

bool FRDriver::discount(uint32_t password,
                        uint64_t sum,
                        uint8_t tax1,
                        uint8_t tax2,
                        uint8_t tax3,
                        uint8_t tax4,
                        const std::string &text)
{
    ByteArray arguments;
    arguments.append<uint32_t>(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(sum, 5, ByteArray::ByteOrder_LittleEndian);
    arguments.append<uint8_t>(tax1);
    arguments.append<uint8_t>(tax2);
    arguments.append<uint8_t>(tax3);
    arguments.append<uint8_t>(tax4);
    arguments.append(
            (const uint8_t*) text.c_str(),
            static_cast<uint32_t>(text.size())
    );

    if (text.size() < 40)
    {
        arguments.appendMultiple<uint8_t>(
                0x00,
                static_cast<uint32_t>(40 - text.size()),
                ByteArray::ByteOrder_LittleEndian
        );
    }

    auto response = sendCommand(Command::Discount, arguments, true);

    return getLastError() == FRDriver::ErrorCode::NoError;
}

uint64_t FRDriver::readLicense(uint32_t sysAdmPassword)
{
    ByteArray arguments;
    arguments.append(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);

    auto response = sendCommand(Command::ReadLicense, arguments, false);

    ByteArrayReader reader(response);
    reader.seek(2);

    return reader.readPart(5,ByteArray::ByteOrder_LittleEndian)
}

static const std::map<int, std::string> errorString = {
        {0x00, "Ошибок нет"},
        {0x01, "Неизвестная команда, неверный формат посылки или неизвестные параметры"},
        {0x02, "Неверное состояние ФН"},
        {0x03, "Ошибка ФН"},
        {0x04, "Ошибка КС"},
        {0x05, "Закончен срок эксплуатации ФН"},
        {0x06, "Архив ФН переполнен"},
        {0x07, "Неверные дата и/или время"},
        {0x08, "Нет запрошенных данных"},
        {0x09, "Некорректное значение параметров команды"},
        {0x10, "Превышение размеров TLV данных"},
        {0x11, "Нет транспортного соединения"},
        {0x12, "Исчерпан ресурс КС (криптографического сопроцессора)"},
        {0x14, "Исчерпан ресурс хранения"},
        {0x15, "Исчерпан ресурс Ожидания передачи сообщения"},
        {0x16, "Продолжительность смены более 24 часов"},
        {0x17, "Неверная разница во времени между 2 операциями"},
        {0x20, "Сообщение от ОФД не может быть принято"},
        {0x2F, "Таймаут обмена с ФН"},
        {0x30, "ФН не отвечает"},
        {0x33, "Некорректные параметры в команде"},
        {0x34, "Нет данных"},
        {0x35, "Некорректный параметр при данных настройках"},
        {0x36, "Некорректные параметры в команде для данной реализации ККТ"},
        {0x37, "Команда не поддерживается в данной реализации ККТ"},
        {0x38, "Ошибка в ПЗУ"},
        {0x39, "Внутренняя ошибка ПО ККТ"},
        {0x3A, "Переполнение накопления по надбавкам в смене"},
        {0x3C, "Смена открыта операция невозможна"},
        {0x3D, "Смена открыта операция невозможна"},
        {0x3E, "Переполнение накопления по секциям в смене"},
        {0x3F, "Переполнение накопления по скидкам в смене"},
        {0x40, "Переполнение диапазона скидок"},
        {0x41, "Переполнение диапазона оплаты наличными"},
        {0x42, "Переполнение диапазона оплаты типом 2"},
        {0x43, "Переполнение диапазона оплаты типом 3"},
        {0x44, "Переполнение диапазона оплаты типом 4"},
        {0x45, "Cумма всех типов оплаты меньше итога чека"},
        {0x46, "Не хватает наличности в кассе"},
        {0x47, "Переполнение накопления по налогам в смене"},
        {0x48, "Переполнение итога чека"},
        {0x49, "Операция невозможна в открытом чеке данного типа"},
        {0x4A, "Открыт чек – операция невозможна"},
        {0x4B, "Буфер чека переполнен"},
        {0x4C, "Переполнение накопления по обороту налогов в смене"},
        {0x4D, "Вносимая безналичной оплатой сумма больше суммы чека"},
        {0x4E, "Смена превысила 24 часа"},
        {0x4F, "Неверный пароль"},
        {0x50, "Идет печать результатов выполнения предыдущей команды"},
        {0x51, "Переполнение накоплений наличными в смене"},
        {0x52, "Переполнение накоплений по типу оплаты 2 в смене"},
        {0x53, "Переполнение накоплений по типу оплаты 3 в смене"},
        {0x54, "Переполнение накоплений по типу оплаты 4 в смене"},
        {0x55, "Чек закрыт – операция невозможна"},
        {0x56, "Нет документа для повтора"},
        {0x58, "Ожидание команды продолжения печати"},
        {0x59, "Документ открыт другим оператором"},
        {0x5B, "Переполнение диапазона надбавок"},
        {0x5C, "Понижено напряжение 24В"},
        {0x5D, "Таблица не определена"},
        {0x5E, "Неверная операция"},
        {0x5F, "Отрицательный итог чека"},
        {0x60, "Переполнение при умножении"},
        {0x61, "Переполнение диапазона цены"},
        {0x62, "Переполнение диапазона количества"},
        {0x63, "Переполнение диапазона отдела"},
        {0x65, "Не хватает денег в секции"},
        {0x66, "Переполнение денег в секции"},
        {0x68, "Не хватает денег по обороту налогов"},
        {0x69, "Переполнение денег по обороту налогов"},
        {0x6A, "Ошибка питания в момент ответа по I2C"},
        {0x6B, "Нет чековой ленты"},
        {0x6D, "Не хватает денег по налогу"},
        {0x6E, "Переполнение денег по налогу"},
        {0x6F, "Переполнение по выплате в смене"},
        {0x71, "Ошибка отрезчика"},
        {0x72, "Команда не поддерживается в данном подрежиме"},
        {0x73, "Команда не поддерживается в данном режиме"},
        {0x74, "Ошибка ОЗУ"},
        {0x75, "Ошибка питания"},
        {0x77, "Ошибка принтера: нет сигнала с датчиков"},
        {0x78, "Замена ПО"},
        {0x7A, "Поле не редактируется"},
        {0x7B, "Ошибка оборудования"},
        {0x7C, "Не совпадает дата"},
        {0x7D, "Неверный формат даты"},
        {0x7E, "Неверное значение в поле длины"},
        {0x7F, "Переполнение диапазона итога чека"},
        {0x84, "Переполнение наличности"},
        {0x85, "Переполнение по продажам в смене"},
        {0x86, "Переполнение по покупкам в смене"},
        {0x87, "Переполнение по возвратам продаж в смене"},
        {0x88, "Переполнение по возвратам покупок в смене"},
        {0x89, "Переполнение по внесению в смене"},
        {0x8A, "Переполнение по надбавкам в чеке"},
        {0x8B, "Переполнение по скидкам в чеке"},
        {0x8C, "Отрицательный итог надбавки в чеке"},
        {0x8D, "Отрицательный итог скидки в чеке"},
        {0x8E, "Нулевой итог чека"},
        {0x90, "Поле превышает размер, установленный в настройках"},
        {0x91, "Выход за границу поля печати при данных настройках шрифта"},
        {0x92, "Наложение полей"},
        {0x93, "Восстановление ОЗУ прошло успешно"},
        {0x94, "Исчерпан лимит операций в чеке"},
        {0xC0, "Контроль даты и времени (подтвердите дату и время)"},
        {0xC2, "Превышение напряжения в блоке питания"},
        {0xC4, "Несовпадение номеров смен"},
        {0xC7, "Поле не редактируется в данном режиме"},
        {0xC8, "Нет связи с принтером или отсутствуют импульсы от таходатчика"},
        {0xFE, "Соединение потеряно"},
        {0xFF, "Неизвестная ошибка"}
};

static const std::map<int, std::string> posModeString = {
        {1, "Выдача данных."},
        {2, "Открытая смена, 24 часа не кончились."},
        {3, "Открытая смена, 24 часа кончились."},
        {4, "Закрытая смена."},
        {5, "Блокировка по неправильному паролю налогового инспектора."},
        {6, "Ожидание подтверждения ввода даты."},
        {7, "Разрешение изменения положения десятичной точки"},
        {8, "Открытый документ"},
        {9, "Режим разрешения технологического обнуления."},
        {10, "Тестовый прогон."},
        {11, "Печать полного фискального отчета."},
        {12, "Зарезервировано."},
        {13, "Работа с фискальным подкладным документом1"},
        {14, "Печать подкладного документа1"},
        {15, "Фискальный подкладной документ сформирован1"}
};

static const std::map<int, std::string> posSubModeString = {
        {0, "Бумага есть"},
        {1, "Пассивное отсутствие бумаги"},
        {2, "Активное отсутствие бумаги"},
        {3, "После активного отсутствия бумаги"},
        {4, "Фаза печати операции полных фискальных отчетов"},
        {5, "Фаза печати операции"}
};

static const std::map<int, std::string> lastPrintResultString = {
        {0, "Печать завершена успешно"},
        {1, "Произочел обрыв бумаги"},
        {2, "Ошибка принтера (перегрев головки, другая ошибка)"},
        {5, "Идет печать"}
};

static const std::map<uint8_t, std::string> fnDocumentString = {
        {0x00, "Нет открытого документа"},
        {0x01, "Отчет о фискализации"},
        {0x02, "Отчет об открытии"},
        {0x04, "Кассовый чек"},
        {0x08, "Отчет о закрытии смены"},
        {0x10, "Отчет о закрытии фискального режима"},
        {0x11, "Бланк строгой отчетности"},
        {0x12, "Отчет об изменении параметров регистрации ККТ в связи с заменой ФН"},
        {0x13, "Отчет об изменении параметров регистрации ККТ"},
        {0x14, "Кассовый чек коррекции"},
        {0x15, "БСО коррекции"},
        {0x17, "Отчет о текущем состоянии расчетов"}
};

static const std::map<uint8_t, std::string> languageString = {
        { 0, "Русский"},
        { 1, "Английский"},
        { 2, "Эстонский"},
        { 3, "Казахский"},
        { 4, "Белорусский"},
        { 5, "Армянский"},
        { 6, "Грузинский"},
        { 7, "Украинский"},
        { 8, "Киргизский"},
        { 9, "Туркменский"},
        {10, "Молдаванский"}
};

std::string FRDriver::Converters::posModeToString(uint8_t mode)
{
    auto pos = posModeString.end();
    if ((pos = posModeString.find(mode)) != posModeString.end())
    {
        return pos->second;
    }

    return "Неизвестный режим: " + std::to_string((int) mode);
}

std::string FRDriver::Converters::errorToString(int error)
{
    auto pos = errorString.end();
    if ((pos = errorString.find((int) error)) != errorString.end())
    {
        return pos->second;
    }

    return "Неизвестная ошибка: " + std::to_string((int) error);
}

std::string FRDriver::Converters::posSubModeToString(uint8_t subMode)
{
    auto pos = posSubModeString.end();
    if ((pos = posSubModeString.find((int) subMode)) != posSubModeString.end())
    {
        return pos->second;
    }

    return "Неизвестный подрежим: " + std::to_string((int) subMode);
}

std::string FRDriver::Converters::lastPrintResultToString(uint8_t result)
{
    auto pos = lastPrintResultString.end();
    if ((pos = lastPrintResultString.find(result)) != lastPrintResultString.end())
    {
        return pos->second;
    }

    return "Неизвестный результат печати: " + std::to_string((int) result);
}

std::string FRDriver::Converters::fnDocumentToString(uint8_t document)
{
    auto pos = fnDocumentString.end();
    if ((pos = fnDocumentString.find(document)) != fnDocumentString.end())
    {
        return pos->second;
    }

    return "Неизвестный тип документа: " + std::to_string((int) document);
}

std::string FRDriver::Converters::deviceLanguageToString(uint8_t language)
{
    auto pos = languageString.end();
    if ((pos = languageString.find(language)) != languageString.end())
    {
        return pos->second;
    }

    return "Неизвестный язык: " + std::to_string(language);
}
