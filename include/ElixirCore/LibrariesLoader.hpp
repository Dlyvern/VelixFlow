#ifndef SCRIPTS_LOADER_HPP
#define SCRIPTS_LOADER_HPP

#include <string>

#ifdef _WIN32
    using LibraryHandle = HMODULE;
#else
using LibraryHandle = void*;
#endif

namespace elix::LibrariesLoader
{
    LibraryHandle loadLibrary(const std::string& libraryPath);
    void* getFunction(const std::string& functionName, LibraryHandle library);
    void closeLibrary(LibraryHandle library);

    template<typename T>
    T getFunction(const std::string& functionName, LibraryHandle library)
    {
        void* rawFunc = getFunction(functionName, library);
        return reinterpret_cast<T>(rawFunc);
    }
} //namespace LibrariesLoader

#endif //SCRIPTS_LOADER_HPP