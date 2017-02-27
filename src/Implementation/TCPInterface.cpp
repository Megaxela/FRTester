//
// Created by megaxela on 08.12.16.
//

#include <chrono>
#include <include/Tools/Time.h>
#include <include/Tools/Logger.h>
#include <include/Tools/byte.h>
#include "include/Implementation/TCPInterface.h"
#include "include/Tools/Platform.h"

#ifdef OS_LINUX
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <zconf.h>
    #include <sys/select.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <arpa/inet.h>

#endif

const int TCPInterface::Type = 3;

TCPInterface::TCPInterface() :
        PhysicalInterface(TCPInterface::Type),
        m_connectionSocket(NetworkTools::invalidSocket)
{

}

TCPInterface::~TCPInterface()
{
    if (m_connectionSocket != INVALID_SOCKET)
    {
        NetworkTools::closeSocket(m_connectionSocket);
    }
}

bool TCPInterface::openConnection()
{
    closeConnection();

    m_connectionSocket = NetworkTools::createTCPSocket();
    if (m_connectionSocket == NetworkTools::invalidSocket)
    {
        Error("Соединение не может быть установлено. Сокет не был создан.");
        return false;
    }

    struct sockaddr_in target;

    target.sin_family = AF_INET;
    target.sin_port = htons(m_port);
    target.sin_addr.s_addr = m_address.data();

    return connect(
            m_connectionSocket,
            (struct sockaddr*) &target,
            sizeof(target)
    ) != SOCKET_ERROR;

}

bool TCPInterface::closeConnection()
{
    if (m_connectionSocket != NetworkTools::invalidSocket)
    {
        NetworkTools::closeSocket(m_connectionSocket);
        m_connectionSocket = NetworkTools::invalidSocket;
    }

    return true;
}

PhysicalInterface::size_t TCPInterface::write(const ByteArray &data)
{
    ExcessLog("Write: " + data.toHex());
    uint32_t i = 0;
    while (i < data.size())
    {
//        if (Time::get<std::chrono::microseconds>() - m_lastByteSendTime < m_byteSendTime)
//        {
//            Time::sleep<std::chrono::microseconds>(
//                    m_byteSendTime - (Time::get<std::chrono::microseconds>() - m_lastByteSendTime)
//            );
//        }

        i += send(m_connectionSocket, (const char*) (data.data() + i), data.length() - i, 0);

//        m_lastByteSendTime = Time::get<std::chrono::microseconds>();
    }

    return data.size();
}

ByteArray TCPInterface::read(const PhysicalInterface::size_t &size, uint32_t timeoutMcs)
{
    // Добавляем наш дескриптов в файл
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    FD_ZERO(&read_fds); // Очищаем набор
    FD_ZERO(&write_fds); // Очищаем набор
    FD_ZERO(&except_fds); // Очищаем набор

    FD_SET(m_connectionSocket, &read_fds);
    FD_SET(m_connectionSocket, &except_fds);

    byte response[size];
    memset(response, 0, size * sizeof(byte));

    size_t dataRead = 0;

    uint32_t timePassed = 0;

    while (dataRead < size)
    {
        auto beginReadTime = Time::get<std::chrono::microseconds>();

        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = timeoutMcs - timePassed;

        int r = select(m_connectionSocket + 1,
                       &read_fds,
                       &write_fds,
                       &except_fds,
                       &timeout);

//        dataRead += ::read(m_fileDescriptor + 1,
//                           response + dataRead,
//                           size - dataRead);
//        timePassed += (uint32_t) (Time::get<std::chrono::microseconds>() - beginReadTime);
        if (r == -1)
        {
            Error("Ошибка select. #" +
                       std::to_string(errno) + ": " +
                       strerror(errno));
            return ByteArray();
        }
        else if (r == 1)
        {
            ssize_t received = recv(
                    m_connectionSocket,
                    (char*) response + dataRead,
                    size - dataRead,
                    0
            );

            if (received == -1)
            {
                Error("Соединение порвано.");
                return ByteArray();
            }

            dataRead += received;

            timePassed += (uint32_t) (Time::get<std::chrono::microseconds>() - beginReadTime);
        }
        else
        {
            Error("Timeout чтения.");
            return ByteArray();
        }
    }

    auto ba = ByteArray(response, static_cast<uint32_t>(dataRead));

    ExcessLog(" READ: " + ba.toHex());

    return ba;
}

void TCPInterface::setAddress(const IPv4Address &address, uint16_t port)
{
    m_address = address;
    m_port = port;
}

IPv4Address TCPInterface::address() const
{
    return m_address;
}

uint16_t TCPInterface::port() const
{
    return m_port;
}
