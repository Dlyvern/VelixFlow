#ifndef BINARY_LOADER_HPP
#define BINARY_LOADER_HPP

#include "DefaultMacros.hpp"
#include <string>
#include <fstream>
#include <vector>

ELIX_NAMESPACE_BEGIN

class BinaryLoader
{
public:
    enum class Mode
    {
        Write,
        Read
    };

    std::streampos getPosition();
    void setPosition(std::streampos pos);
    void seek(std::streamoff offset); 

    bool init(const std::string& filePath, const Mode& mode);

    void close();

    template<typename T>
    void write(const T& data)
    {
        static_assert(std::is_trivially_copyable<T>::value, "Only trivially copyable types are supported.");
        m_file.write(reinterpret_cast<const char*>(&data), sizeof(T));
    }

    template<typename T>
    void read(T& data)
    {
        static_assert(std::is_trivially_copyable<T>::value, "Only trivially copyable types are supported.");
        m_file.read(reinterpret_cast<char*>(&data), sizeof(T));
    }

    void write(const std::string& str);

    void read(std::string& str);

    template<typename T>
    void write(const std::vector<T>& vec)
    {
        uint32_t size = static_cast<uint32_t>(vec.size());
        write(size);
        for (const auto& item : vec)
            write(item);
    }
    
    template<typename T>
    void read(std::vector<T>& vec)
    {
        uint32_t size;
        read(size);
        vec.resize(size);

        for (auto& item : vec)
            read(item);
    }

    ~BinaryLoader();
private:
    std::fstream m_file;
    Mode m_mode;
};

ELIX_NAMESPACE_END

#endif //BINARY_LOADER_HPP
