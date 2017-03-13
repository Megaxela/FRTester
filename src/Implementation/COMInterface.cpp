//
// Created by megaxela on 14.11.16.
//

#include "Implementation/COMInterface.h"
#include "Tools/Logger.h"

const int COMInterface::Type = 1;

COMInterface::COMInterface() :
    PhysicalInterface(COMInterface::Type),
    m_device(),
    m_baudRate(2400),
    m_byteSendTime(20000),
    m_lastByteSendTime(Time::get<std::chrono::microseconds>())
{

}

COMInterface::~COMInterface()
{

}

bool COMInterface::openConnection()
{
    if (m_device.empty() || m_baudRate == 0)
    {
        Error("Попытка открыть соединение без указания устройства или скорости обмена данными.");
        return false;
    }

    return TTY::connect(m_device, m_baudRate) == 0;
}

bool COMInterface::closeConnection()
{
    TTY::disconnect();

    return true;
}

PhysicalInterface::size_t COMInterface::write(const ByteArray &data)
{
    ExcessLog("Write: " + data.toHex());
    for (uint32_t i = 0; i < data.length(); )
    {
        if (Time::get<std::chrono::microseconds>() - m_lastByteSendTime < m_byteSendTime)
        {
            Time::sleep<std::chrono::microseconds>(
                    m_byteSendTime - (Time::get<std::chrono::microseconds>() - m_lastByteSendTime)
            );
        }

        i += TTY::write(data.mid(i, 1));

        m_lastByteSendTime = Time::get<std::chrono::microseconds>();
    }

    return data.size();
}

ByteArray COMInterface::read(const size_t &n,
                             uint32_t timeout)
{
    auto r = TTY::read(n, timeout);

    ExcessLog("Read: " + r.toHex());

    return r;
}

void COMInterface::setDevice(const std::string &name)
{
    m_device = name;
}

std::string COMInterface::device() const
{
    return m_device;
}

void COMInterface::setBaudRate(int rate)
{
    m_baudRate = rate;
}

int COMInterface::baudRate() const
{
    return m_baudRate;
}

void COMInterface::setByteSendTime(Time::time_t time)
{
    m_byteSendTime = time;
}
