#ifndef NET_ADDRESS_HPP
#define NET_ADDRESS_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include <string>
#include <cstdint>

ELIX_NAMESPACE_BEGIN

namespace net
{
    struct NetAddress
    {
        std::string ip;
        uint16_t port;
    };

    struct NetData
    {
        const uint8_t* data;
        size_t size;
        NetAddress address;
    };
} //namespace net

ELIX_NAMESPACE_END

#endif //NET_ADDRESS_HPP