//
// Created by megaxela on 08.12.16.
//

#include "include/Tools/NetworkTools.h"
#include "include/Tools/Platform.h"

#include <include/Tools/Logger.h>
#ifdef OS_LINUX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <include/Tools/IPv4Address.h>
    #include <zconf.h>

#endif

#define NO_FLAGS

bool NetworkTools::initSockets()
{
#ifdef OS_WINDOWS
    WSAData data;

    int result = WSAStartup(MAKEWORD(2, 2), &data);
    if (result != 0)
    {
        Critical("WSAStartup fail. Error: " + std::to_string(result));
        return false;
    }
#endif

    return true;
}

NetworkTools::socket_t NetworkTools::createUDPSocket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

NetworkTools::socket_t NetworkTools::createTCPSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

bool NetworkTools::bind(NetworkTools::socket_t socket, IPv4Address address, uint16_t port)
{
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = port;
    server.sin_addr.s_addr = address.data();

    return ::bind(socket, (struct sockaddr*) &server, sizeof(sockaddr_in)) != SOCKET_ERROR;
}

void NetworkTools::closeSocket(NetworkTools::socket_t socket)
{
#ifdef OS_WINDOWS
    closesocket(socket);
#endif
#ifdef OS_LINUX
    close(socket);
#endif
}

void ::NetworkTools::cleanupSockets()
{
#ifdef OS_WINDOWS
    WSACleanup();
#endif
}
