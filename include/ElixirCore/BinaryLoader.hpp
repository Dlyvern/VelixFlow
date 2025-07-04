#ifndef BINARY_LOADER_HPP
#define BINARY_LOADER_HPP

#include "DefaultMacros.hpp"
#include <string>
#include <fstream>

ELIX_NAMESPACE_BEGIN

class BinaryLoader
{
public:
    enum class Mode
    {
        Write,
        Read
    };

    BinaryLoader(const std::string& filePath, const Mode& mode);

    void close();

    ~BinaryLoader();
private:
    std::fstream m_file;
};

ELIX_NAMESPACE_END

#endif //BINARY_LOADER_HPP
