//
// Created by megaxela on 08.12.16.
//

#include <chrono>
#include <Tools/Time.h>
#include <Tools/Logger.h>
#include <Tools/byte.h>
#include <Implementation/TCPInterface.h>
#include <Tools/Platform.h>

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
        m_connectionSocket(NetworkTools::invalidSocket),
        m_address(),
        m_port(0)
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
    ExcessLog("-> " + data.toHex());
    uint32_t i = 0;
    while (i < data.size())
    {
        i += send(m_connectionSocket, (const char*) (data.data() + i), data.length() - i, 0);
    }

    return data.size();
}

ByteArray TCPInterface::read(const PhysicalInterface::size_t &size, uint32_t timeoutMcs)
{
    LogStream() << "Запрашиваю " << size << " байт с таймаутом в " << timeoutMcs / 1000.0 << " миллисекунд." << std::endl;

    auto beginReadTime = Time::get<std::chrono::microseconds>();
    // Добавляем наш дескриптов в файл
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    FD_ZERO(&read_fds); // Очищаем набор
    FD_ZERO(&write_fds); // Очищаем набор
    FD_ZERO(&except_fds); // Очищаем набор

    FD_SET(m_connectionSocket, &read_fds);
    FD_SET(m_connectionSocket, &except_fds);

    byte* response = new byte[size];
    memset(response, 0, size * sizeof(byte));

    size_t dataRead = 0;

    while (dataRead < size)
    {

        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = timeoutMcs - ((Time::getMicroseconds() - beginReadTime));

        int r = select(m_connectionSocket + 1,
                       &read_fds,
                       &write_fds,
                       &except_fds,
                       &timeout);

        if (r == -1)
        {
            Error("Ошибка select. #" +
                       std::to_string(errno) + ": " +
                       strerror(errno));
            delete[] response;
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
                delete[] response;
                return ByteArray();
            }

            dataRead += received;
        }
        else
        {
            Error("Timeout чтения.");
            delete[] response;
            return ByteArray();
        }
    }

    if (dataRead == size)
    {
        LogStream() << "Чтение успешно завершено за "
                    << (Time::getMicroseconds() - beginReadTime) / 1000.0
                    << " миллисекунд."
                    << std::endl;
    }
    else
    {
        ErrorStream() << "Чтение не полностью завершено за "
                      << (Time::getMicroseconds() - beginReadTime) / 1000.0
                      << " миллисекунд."
                      << std::endl;
    }

    auto ba = ByteArray(response, static_cast<uint32_t>(dataRead));

    ExcessLog("<- " + ba.toHex());
    delete[] response;

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
