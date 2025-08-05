#include "VelixFlow/Net/NetStream.hpp"

ELIX_NAMESPACE_BEGIN

net::NetStream::NetStream(const net::NetStream::Mode& mode) : m_mode(mode) {}

const std::vector<uint8_t>& net::NetStream::getBuffer() const
{
    return m_buffer;
}

const uint8_t* net::NetStream::data() const
{
    return m_buffer.data();
}

size_t net::NetStream::size() const
{
    return m_buffer.size();
}

ELIX_NAMESPACE_END