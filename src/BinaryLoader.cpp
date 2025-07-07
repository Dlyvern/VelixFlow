#include "BinaryLoader.hpp"

#include <cassert>

ELIX_NAMESPACE_BEGIN

bool BinaryLoader::init(const std::string& filePath, const Mode& mode)
{
    m_mode = mode;

    std::ios_base::openmode openMode = (m_mode == Mode::Write) ? std::ios::out | std::ios::binary : std::ios::in | std::ios::binary;

    m_file.open(filePath, openMode);

    if(!m_file.is_open())
        return false;

    return true;
}

void BinaryLoader::close()
{
    if(m_file.is_open())
        m_file.close();
}

BinaryLoader::~BinaryLoader()
{
    close();
}

std::streampos BinaryLoader::getPosition() 
{
    return m_file.tellg();
}

void BinaryLoader::setPosition(std::streampos pos) 
{
    if (m_mode == Mode::Read)
        m_file.seekg(pos);
    else
        m_file.seekp(pos);
}

void BinaryLoader::seek(std::streamoff offset) 
{
    if (m_mode == Mode::Read)
        m_file.seekg(offset, std::ios::cur);
    else
        m_file.seekp(offset, std::ios::cur);
}

void BinaryLoader::write(const std::string& str) 
{
    uint32_t size = static_cast<uint32_t>(str.size());
    write(size);
    m_file.write(str.c_str(), size);
}

void BinaryLoader::read(std::string& str)
{
    uint32_t size;
    read(size);
    str.resize(size);
    m_file.read(&str[0], size);
}


ELIX_NAMESPACE_END
