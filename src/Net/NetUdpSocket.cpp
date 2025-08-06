#include "VelixFlow/Net/NetUdpSocket.hpp"
#include "VelixFlow/Logger.hpp"

ELIX_NAMESPACE_BEGIN

namespace net
{
    NetUdpSocket::NetUdpSocket()
    {
#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
        m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if(m_socket == -1)
            ELIX_LOG_WARN("Failed to create a new socket");
    }   

    NetUdpSocket::~NetUdpSocket()
    {
        if(m_socket != -1)
        {
#ifdef _WIN32
            closesocket(m_socket);
            WSACleanup();
#else
            close(m_socket);
#endif
        }
    }

    bool NetUdpSocket::bind(uint16_t port)
    {
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.s_addr = INADDR_ANY;

        return ::bind(m_socket, (sockaddr*)&address, sizeof(address)) == 0;
    }

    void NetUdpSocket::update()
    {
        uint8_t buffer[1024];
        sockaddr_in from{};
        socklen_t fromLen = sizeof(from);

        // int received = recvfrom(m_socket, (char*)buffer, sizeof(buffer), MSG_DONTWAIT, (sockaddr*)&from, &fromLen);
        int received{0};
        if(received <= 0)
            return;

        char ipStr[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &from.sin_addr, ipStr, sizeof(ipStr));
        uint16_t port = htons(from.sin_port);

        NetAddress address{ipStr, port};

        NetData data;

        data.address = address;
        data.data = buffer;
        data.size = received;

        newDataReceived.emit(data);
    }

    bool NetUdpSocket::send(const uint8_t* data, size_t size, const NetAddress& to)
    {
        sockaddr_in address{};

        address.sin_family = AF_INET;
        address.sin_port = htons(to.port);
        inet_pton(AF_INET, to.ip.c_str(), &address.sin_addr);

        return sendto(m_socket, (const char*)data, (int)size, 0, (sockaddr*)&address, sizeof(address) >= 0);
    }
}

ELIX_NAMESPACE_END
