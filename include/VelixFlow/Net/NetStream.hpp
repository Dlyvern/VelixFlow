#ifndef NET_STREAM_HPP
#define NET_STREAM_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include <vector>
#include <string>
#include "VelixFlow/Logger.hpp"
#include <shared_mutex>
#include <cstring>

//TODO add thread-safe

ELIX_NAMESPACE_BEGIN

namespace net
{
    class NetStream
    {
    public:
        enum class Mode
        {
            READ,
            WRITE
        };

        explicit NetStream(const Mode& mode);

        const std::vector<uint8_t>& getBuffer() const;
        const uint8_t* data() const;
        size_t size() const;


        template<typename T>
        NetStream& operator<<(const T& value) 
        {
            static_assert(std::is_trivially_copyable<T>::value, "Only POD types allowed");

            if (m_mode != Mode::WRITE) 
            {
                ELIX_LOG_ERROR("Stream not in write mode");
                return *this;
            }

            const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&value);
            m_buffer.insert(m_buffer.end(), ptr, ptr + sizeof(T));
            return *this;
        }

        template<typename T>
        NetStream& operator>>(T& value) 
        {
            static_assert(std::is_trivially_copyable<T>::value, "Only POD types allowed");

            if (m_mode != Mode::READ)
            {
                ELIX_LOG_ERROR("Stream not in read mode");
                return *this;
            } 

            if (m_cursor + sizeof(T) > m_buffer.size()) 
            {
                ELIX_LOG_ERROR("Stream read overflow");
                return *this;
            }
            
            std::memcpy(&value, m_buffer.data() + m_cursor, sizeof(T));
            m_cursor += sizeof(T);
            return *this;
        }

        NetStream& operator<<(const std::string& str) 
        {
            if (m_mode != Mode::WRITE)
            {
                ELIX_LOG_ERROR("Stream not in write mode");
                return *this;
            }

            uint16_t len = static_cast<uint16_t>(str.size());
            *this << len;
            m_buffer.insert(m_buffer.end(), str.begin(), str.end());
            return *this;
        }

        NetStream& operator>>(std::string& str) 
        {
            if (m_mode != Mode::READ)
            {
                ELIX_LOG_ERROR("Stream not in read mode");
                return *this;
            }

            uint16_t len;
            *this >> len;

            if (m_cursor + len > m_buffer.size())
            {
                ELIX_LOG_ERROR("Stream read overflow");
                return *this;
            }

            str.assign(reinterpret_cast<const char*>(m_buffer.data() + m_cursor), len);
            m_cursor += len;
            return *this;
        }

    private:
        Mode m_mode;
        std::vector<uint8_t> m_buffer;
        size_t m_cursor{0};
        mutable std::shared_mutex m_mutex;
    };

} //namespace net
ELIX_NAMESPACE_END

#endif //NET_STREAM_HPP