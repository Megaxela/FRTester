//
// Created by megaxela on 08.12.16.
//

#ifndef FRTESTER_NETWORKTOOLS_H
#define FRTESTER_NETWORKTOOLS_H


#include <Tools/Platform.h>
#include <Tools/IPv4Address.h>
#include <cstdint>

#ifdef OS_WINDOWS
#include <winsock2.h>
#endif
#ifdef OS_LINUX
    #define SOCKET_ERROR -1
    #define INVALID_SOCKET -1
#endif

namespace NetworkTools
{
#ifdef OS_WINDOWS
    typedef SOCKET socket_t;
    const socket_t invalidSocket = INVALID_SOCKET;
#else
    typedef int socket_t;
    const socket_t invalidSocket = -1;
#endif

    /**
     * @brief Method for initializing sockets.
     * Windows using WSA init.
     * @return
     */
    bool initSockets();

    /**
     * @brief Method for uninitializing sockets.
     * Windows using WSA cleanup.
     */
    void cleanupSockets();

    /**
     * @brief Method for creating UDP socket.
     * @return
     */
    socket_t createUDPSocket();

    /**
     * @brief Method for creating TCP socket.
     * @return Created socket.
     */
    socket_t createTCPSocket();

    /**
     * @brief Method for binding socket to port.
     * @param socket Socket to bind.
     * @param address Address for binding.
     * @param port Target port.
     * @return Success of binding.
     */
    bool bind(socket_t socket, IPv4Address address, uint16_t port);

    /**
     * @brief Function for closing socket.
     * @param socket Socket to be closed.
     */
    void closeSocket(socket_t socket);
}



#endif //FRTESTER_NETWORKTOOLS_H
