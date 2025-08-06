#ifndef NET_UDP_SOCKET_HPP
#define NET_UDP_SOCKET_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Net/NetAddress.hpp"
#include "VelixFlow/Signal.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using socklen_t = int;
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

ELIX_NAMESPACE_BEGIN

namespace net
{
    class NetUdpSocket
    {
    public:
        Signal<NetData> newDataReceived;
        
        NetUdpSocket();

        ~NetUdpSocket();

        bool bind(uint16_t port);

        bool send(const uint8_t* data, size_t size, const NetAddress& to);

        void update();
    private:
        int m_socket{-1};
    };

} //namespace net
ELIX_NAMESPACE_END

#endif //NET_UDP_SOCKET_HPP