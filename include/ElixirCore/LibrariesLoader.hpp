#ifndef SCRIPTS_LOADER_HPP
#define SCRIPTS_LOADER_HPP

#include <string>

namespace elix::LibrariesLoader
{
    void* loadLibrary(const std::string& libraryPath);
    void* getFunction(const std::string& functionName, void* library);
    void closeLibrary(void* library);
} //namespace LibrariesLoader

#endif //SCRIPTS_LOADER_HPP