//
// Created by megaxela on 11.11.16.
//

#include <map>
#include <include/Tools/ByteArrayReader.h>
#include "FRDriver.h"
#include "Abstract/PhysicalInterface.h"
#include "Tools/Logger.h"
#include "Tools/Time.h"

FRDriver::FRDriver() :
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

    m_protocol->prepareDeviceToWrite(m_interface);

    // Оборачиваем данные
    auto formedData = m_protocol->proceedData(data);

    // Отправляем на ФР
    auto sent = 0;

    auto sendTime = Time::timeFunction<std::chrono::microseconds>(
            [&]()
            {
                sent = m_interface->write(formedData);
            }
    );

    ExcessLog("Время отправки заняло: " + std::to_string(sendTime) + " микросекунд.");

    auto byteSendTimeMcs = sendTime / formedData.length();

    if (byteSendTimeMcs == 0)
    {
        Warning("Вычисленное время отправки одного байта равно нулю. Во избежание ошибок меняем его на 2мс.");
        byteSendTimeMcs = 2000;
    }

    // Проверяем количество отправленных данных
    if (sent < formedData.length())
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

ByteArray FRDriver::sendCommand(const FRDriver::Command &c, const ByteArray &arguments)
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
    if (data.length() < commandSize + 1)
    {
        Error("Попытка обработать ответ неверной длины. Меняю последнюю ошибку на Unknown.");
        m_lastErrorCode = ErrorCode::Unknown;
        return;
    }

    m_lastErrorCode = static_cast<ErrorCode>(data[commandSize]);

    Log("Получена ошибка: #" + std::to_string(data[commandSize]) + " из ответа " + data.toHex());

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

    ByteArray data = sendCommand(Command::Beep, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::Sell, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::ReturnSell, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::ReturnBuy, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::Buy, arguments);

    proceedResponse(data);

    return m_lastErrorCode == ErrorCode::NoError;
}



bool FRDriver::openShift(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::OpenShift, arguments);

    proceedResponse(data);

    return m_lastErrorCode == ErrorCode::NoError;
}

FRDriver::ShortState FRDriver::shortStateRequest(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ShortStateRequest, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::ShiftCloseReport, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::CloseCheck, arguments);

    proceedResponse(data);

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

bool FRDriver::resumePrinting(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ContinuePrint,
                                 arguments);

    proceedResponse(data);

    return m_lastErrorCode == ErrorCode::NoError;
}

FRDriver::FullState FRDriver::fullStateRequest(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::FullStateRequest, arguments);

    proceedResponse(data);

    FullState state = FRDriver::FullState();

    if (m_lastErrorCode != ErrorCode::NoError)
    {
        return state;
    }

    ByteArrayReader reader(data);
    reader.seek(3);

    state.firmwareVersion.major             = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareVersion.minor             = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareBuild                     = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    state.firmwareDate.day                  = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareDate.month                = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.firmwareDate.year                 = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.numberInHall                      = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
    state.currentDocumentPassthrougNumber   = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    state.posFlags                          = reader.read<uint16_t>(ByteArray::ByteOrder_LittleEndian);
    state.posMode                           = reader.read<uint8_t> (ByteArray::ByteOrder_LittleEndian);
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

    ByteArray data = sendCommand(Command::CancelCheck, arguments);

    proceedResponse(data);

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

    ByteArray data = sendCommand(Command::OperatingRegisterRequest, arguments);

    proceedResponse(data, true);

    return data.read<uint16_t>(3, ByteArray::ByteOrder_LittleEndian);
}

FRDriver::ExchangeConfiguration FRDriver::readExchangeConfiguration(uint32_t sysAdmPassword, uint8_t portNumber)
{
    ByteArray arguments;

    arguments.append(sysAdmPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(portNumber,     ByteArray::ByteOrder_LittleEndian);

    ByteArray data = sendCommand(Command::ReadExchangeConfiguration, arguments);

    proceedResponse(data, false);

    ExchangeConfiguration configuration;

    configuration.baudRateCode = data.read<uint8_t>(2);
    configuration.byteTimeout = data.read<uint8_t>(3);

    return configuration;
}

bool FRDriver::technologicalReset()
{
    ByteArray data = sendCommand(Command::TechReset);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::StandardStringPrint, arguments);

    proceedResponse(data, true);

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

    auto data = sendCommand(Command::DocumentHeaderPrint, arguments);

    proceedResponse(data, true);

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

    auto data = sendCommand(Command::CurrencyRegisterRequest, arguments);

    proceedResponse(data, true);

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
    arguments.append(row);
    arguments.append(field);
    arguments.append((uint8_t*) value.c_str(),
                     (uint32_t) value.length());

    if (value.length() < 40)
    {
        arguments.appendMultiple<uint8_t>(0x00, (uint32_t) (40 - value.length()));
    }

    auto data = sendCommand(Command::WriteTable, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::TimeProgramming, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::DateProgramming, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::DateConfirm, arguments);

    proceedResponse(data, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::tableValuesInit(uint32_t sysPassword)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::TableValuesInit, arguments);

    proceedResponse(data, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::cutCheck(uint32_t sysPassword, uint8_t cutType)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(cutType);

    auto data = sendCommand(Command::CutCheck, arguments);

    proceedResponse(data, false);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::FontConfiguration FRDriver::readFontConfiguration(uint32_t sysPassword,
                                                            uint8_t fontNumber)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(fontNumber);

    auto data = sendCommand(Command::ReadFontConfiguration, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::TotalExtinction, arguments);

    proceedResponse(data, false);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::scrolling(uint32_t password, uint8_t flags, uint8_t numberOfLines)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(flags);
    arguments.append(numberOfLines);

    auto data = sendCommand(Command::Scrolling, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::TableStructure FRDriver::tableStructureRequest(uint32_t sysPassword,
                                                         uint8_t tableNumber)
{
    ByteArray arguments;

    arguments.append(sysPassword, ByteArray::ByteOrder_LittleEndian);
    arguments.append(tableNumber);

    auto data = sendCommand(Command::TableStructureRequest, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::FieldStructureRequest, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::FontStringPrint, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::shiftReportWithoutExtinction(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::ShiftReportNoExtinction, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::sectionsReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::SectionsReport, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::taxesReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::TaxesReport, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::cashierReport(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::CashierReport, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

uint16_t FRDriver::payin(uint32_t password, uint64_t sum)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.appendPart(sum, 5, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PayIn, arguments);

    proceedResponse(data, true);

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

    auto data = sendCommand(Command::PayOut, arguments);

    proceedResponse(data, true);

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

    auto data = sendCommand(Command::PrintCliches, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::printDocumentEnd(uint32_t password, uint8_t printAds)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(printAds);

    auto data = sendCommand(Command::DocumentEndPrint, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::printAds(uint32_t password)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::PrintAds, arguments);

    proceedResponse(data, true);

    return getLastError() == ErrorCode::NoError;
}

bool FRDriver::enterFactoryNumber(uint32_t password, uint32_t factoryNumber)
{
    ByteArray arguments;

    arguments.append(password, ByteArray::ByteOrder_LittleEndian);
    arguments.append(factoryNumber, ByteArray::ByteOrder_LittleEndian);

    auto data = sendCommand(Command::EnterFactoryNumber, arguments);

    proceedResponse(data, false);

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

    auto data = sendCommand(Command::GetInformationExchangeStatus, arguments);

    proceedResponse(data, false);

    InformExchangeStatus status = InformExchangeStatus();

    Log("Data: " + data.toHex());

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

    ByteArray data = sendCommand(Command::ReadTable, arguments);

    proceedResponse(data, true);

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

    ByteArray data = sendCommand(Command::ReadTable, arguments);

    proceedResponse(data, true);

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

    auto data = sendCommand(Command::WriteTable, arguments);

    proceedResponse(data, false);

    return getLastError() == ErrorCode::NoError;
}

FRDriver::NonZeroSums FRDriver::getNonZeroSums()
{
    ByteArray arguments;

    arguments.append((const uint8_t*) "\xF4\x00\x00\x00\x00", 5);

    auto data = sendCommand(Command::NonZeroSums, arguments);

    proceedResponse(data, false);

    NonZeroSums sums = FRDriver::NonZeroSums();

    if (data.length() != 34)
    {
        return sums;
    }

    ByteArrayReader reader(data);
    reader.seek(2);

    sums.firstSum  = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);
    sums.secondSum = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);
    sums.thirdSum  = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);
    sums.fourthSum = reader.read<uint64_t>(ByteArray::ByteOrder_LittleEndian);

    return sums;
}

static std::map<int, std::string> errorString = {
        {0x00, "ФН Успешное выполнение команды"},
        {0x01, "ФН Неизвестная команда, неверный форматпосылки или неизвестные параметры."},
        {0x02, "ФН Неверное состояние ФН"},
        {0x03, "ФН Ошибка ФН"},
        {0x04, "ФН Ошибка КС"},
        {0x05, "ФН Закончен срок эксплуатации ФН"},
        {0x06, "ФН Архив ФН переполнен"},
        {0x07, "ФН Неверные дата и/или время"},
        {0x08, "ФН Нет запрошенных данных"},
        {0x09, "ФН Некорректное значение параметров команды"},
        {0x10, "ФН Превышение размеров TLV данных"},
        {0x11, "ФН Нет транспортного соединения"},
        {0x12, "ФН Исчерпан ресурс КС"},
        {0x14, "ФН Исчерпан ресурс хранения"},
        {0x15, "ФН Исчерпан ресурс Ожидания передачи сообщения"},
        {0x16, "ФН Продолжительность смены более 24 часов"},
        {0x17, "ФН Неверная разница во времени между 2 операциями."},
        {0x20, "ФН Сообщение от ОФД не может быть принято"},
        {0x26, "ККТ Вносимая клиентом сумма меньше суммы чека"},
        {0x2B, "ККТ Невозможно отменить предыдущую команду"},
        {0x2C, "ККТ Обнулённая касса (повторное гашение невозможно)"},
        {0x2D, "ККТ Сумма чека по секции меньше суммы сторно"},
        {0x2E, "ККТ В ККТ нет денег для выплаты"},
        {0x30, "ККТ ККТ заблокирован, ждет ввода пароля налогового инспектора"},
        {0x32, "ККТ Требуется выполнение общего гашения"},
        {0x33, "ККТ Некорректные параметры в команде"},
        {0x34, "ККТ Нет данных"},
        {0x35, "ККТ Некорректный параметр при данных настройках"},
        {0x36, "ККТ Некорректные параметры в команде для данной реализации ККТ"},
        {0x37, "ККТ Команда не поддерживается в данной реализации ККТ"},
        {0x38, "ККТ Ошибка в ПЗУ"},
        {0x39, "ККТ Внутренняя ошибка ПО ККТ"},
        {0x3A, "ККТ Переполнение накопления по надбавкам в смене"},
        {0x3B, "ККТ Переполнение накопления в смене"},
        {0x3C, "ККТ Смена открыта – операция невозможна"},
        {0x3D, "ККТ Смена не открыта – операция невозможна"},
        {0x3E, "ККТ Переполнение накопления по секциям всмене"},
        {0x3F, "ККТ Переполнение накопления по скидкам всмене"},
        {0x40, "ККТ Переполнение диапазона скидок"},
        {0x41, "ККТ Переполнение диапазона оплаты наличными"},
        {0x42, "ККТ Переполнение диапазона оплаты типом 2"},
        {0x43, "ККТ Переполнение диапазона оплаты типом 3"},
        {0x44, "ККТ Переполнение диапазона оплаты типом 4"},
        {0x45, "ККТ Cумма всех типов оплаты меньше итога чека"},
        {0x46, "ККТ Не хватает наличности в кассе"},
        {0x47, "ККТ Переполнение накопления по налогам в смене"},
        {0x48, "ККТ Переполнение итога чека"},
        {0x49, "ККТ Операция невозможна в открытом чеке данного типа"},
        {0x4A, "ККТ Открыт чек – операция невозможна"},
        {0x4B, "ККТ Буфер чека переполнен"},
        {0x4C, "ККТ Переполнение накопления по обороту налогов в смене"},
        {0x4D, "ККТ Вносимая безналичной оплатой сумма больше суммы чека"},
        {0x4E, "ККТ Смена превысила 24 часа"},
        {0x4F, "ККТ Неверный пароль"},
        {0x50, "ККТ Идет печать результатов выполнения предыдущей команды"},
        {0x51, "ККТ Переполнение накоплений наличными в смене"},
        {0x52, "ККТ Переполнение накоплений по типу оплаты 2 в смене"},
        {0x53, "ККТ Переполнение накоплений по типу оплаты 3 в смене"},
        {0x54, "ККТ Переполнение накоплений по типу оплаты 4 в смене"},
        {0x55, "ККТ Чек закрыт – операция невозможна"},
        {0x56, "ККТ Нет документа для повтора"},
        {0x58, "ККТ Ожидание команды продолжения печати"},
        {0x59, "ККТ Документ открыт другим кассиром"},
        {0x5A, "ККТ Скидка превышает накопления в чеке"},
        {0x5B, "ККТ Переполнение диапазона надбавок"},
        {0x5C, "ККТ Понижено напряжение 24В"},
        {0x5D, "ККТ Таблица не определена"},
        {0x5E, "ККТ Неверная операция"},
        {0x5F, "ККТ Отрицательный итог чека"},
        {0x60, "ККТ Переполнение при умножении"},
        {0x61, "ККТ Переполнение диапазона цены"},
        {0x62, "ККТ Переполнение диапазона количества"},
        {0x63, "ККТ Переполнение диапазона отдела"},
        {0x65, "ККТ Не хватает денег в секции"},
        {0x66, "ККТ Переполнение денег в секции"},
        {0x68, "ККТ Не хватает денег по обороту налогов"},
        {0x69, "ККТ Переполнение денег по обороту налогов"},
        {0x6A, "ККТ Ошибка питания в момент ответа по I2C"},
        {0x6B, "ККТ Нет чековой ленты"},
        {0x6C, "ККТ Нет операционного журнала"},
        {0x6D, "ККТ Не хватает денег по налогу"},
        {0x6E, "ККТ Переполнение денег по налогу"},
        {0x6F, "ККТ Переполнение по выплате в смене"},
        {0x71, "ККТ Ошибка отрезчика"},
        {0x72, "ККТ Команда не поддерживается в данном подрежиме"},
        {0x73, "ККТ Команда не поддерживается в данном режиме"},
        {0x74, "ККТ Ошибка ОЗУ"},
        {0x75, "ККТ Ошибка питания"},
        {0x76, "ККТ Ошибка принтера: нет импульсов с тахогенератора"},
        {0x77, "ККТ Ошибка принтера: нет сигнала с датчиков"},
        {0x78, "ККТ Замена ПО"},
        {0x7A, "ККТ Поле не редактируется"},
        {0x7B, "ККТ Ошибка оборудования"},
        {0x7C, "ККТ Не совпадает дата"},
        {0x7D, "ККТ Неверный формат даты"},
        {0x7E, "ККТ Неверное значение в поле длины"},
        {0x7F, "ККТ Переполнение диапазона итога чека"},
        {0x84, "ККТ Переполнение наличности"},
        {0x85, "ККТ Переполнение по приходу в смене"},
        {0x86, "ККТ Переполнение по расходу в смене"},
        {0x87, "ККТ Переполнение по возвратам прихода в смене"},
        {0x88, "ККТ Переполнение по возвратам расхода в смене"},
        {0x89, "ККТ Переполнение по внесению в смене"},
        {0x8A, "ККТ Переполнение по надбавкам в чеке"},
        {0x8B, "ККТ Переполнение по скидкам в чеке"},
        {0x8C, "ККТ Отрицательный итог надбавки в чеке"},
        {0x8D, "ККТ Отрицательный итог скидки в чеке"},
        {0x8E, "ККТ Нулевой итог чека"},
        {0x8F, "ККТ Касса не зарегистрирована"},
        {0x90, "ККТ Поле превышает размер, установленный в настройках"},
        {0x91, "ККТ Выход за границу поля печати при данных настройках шрифта"},
        {0x92, "ККТ Наложение полей"},
        {0x93, "ККТ Восстановление ОЗУ прошло успешно"},
        {0x94, "ККТ Исчерпан лимит операций в чеке"},
        {0x96, "ККТ Выполните отчет о закрытии смены"},
        {0x9B, "ККТ Некорректное действие"},
        {0x9C, "ККТ Товар не найден по коду в базе товаров"},
        {0x9D, "ККТ Неверные данные в записе о товаре в базе товаров"},
        {0x9E, "ККТ Неверный размер файла базы или регистров товаров"},
        {0xC0, "ККТ Контроль даты и времени (подтвердите дату и время)"},
        {0xC2, "ККТ Превышение напряжения в блоке питания"},
        {0xC4, "ККТ Несовпадение номеров смен"},
        {0xC5, "ККТ Буфер подкладного документа пуст"},
        {0xC6, "ККТ Подкладной документ отсутствует"},
        {0xC7, "ККТ Поле не редактируется в данном режиме"},
        {0xC8, "ККТ Нет связи с принтером или отсутствуют импульсы от таходатчика"},
        {0xC9, "ККТ Перегрев печатающей головки"},
        {0xCA, "ККТ Температура вне условий эксплуатации"},
        {0xCB, "ККТ Неверный подытог чека"},
        {0xCE, "ККТ Лимит минимального свободного объема"},
//        {0xОЗ, "ПЗУ на ККТ исчерпан"},
        {0xCF, "ККТ Неверная дата (Часы сброшены? Установите дату!)"},
        {0xD0, "ККТ Отчет операционного журнала не распечатан!"},
        {0xD1, "ККТ Нет данных в буфере"},
        {0xD5, "ККТ Критическая ошибка при загрузке ERRxx"},
        {0xE0, "ККТ Ошибка связи с купюроприемником"},
        {0xE1, "ККТ Купюроприемник занят"},
        {0xE2, "ККТ Итог чека не соответствует итогу купюроприемника"},
        {0xE3, "ККТ Ошибка купюроприемника"},
        {0xE4, "ККТ Итог купюроприемника не нулевой"},
        {0xFF, "Неизвестная ошибка"}
};

std::map<int, std::string> posModeString = {
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

std::map<int, std::string> posSubModeString = {
        {0, "Бумага есть"},
        {1, "Пассивное отсутствие бумаги"},
        {2, "Активное отсутствие бумаги"},
        {3, "После активного отсутствия бумаги"},
        {4, "Фаза печати операции полных фискальных отчетов"},
        {5, "Фаза печати операции"}
};

std::map<int, std::string> lastPrintResultString = {
        {0, "Печать завершена успешно"},
        {1, "Произочел обрыв бумаги"},
        {2, "Ошибка принтера (перегрев головки, другая ошибка)"},
        {5, "Идет печать"}
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
