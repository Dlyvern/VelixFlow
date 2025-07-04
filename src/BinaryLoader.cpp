#include "BinaryLoader.hpp"

ELIX_NAMESPACE_BEGIN

BinaryLoader::BinaryLoader(const std::string& filePath, const Mode& mode)
{

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

ELIX_NAMESPACE_END
