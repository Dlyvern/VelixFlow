#ifndef SCRIPTS_LOADER_HPP
#define SCRIPTS_LOADER_HPP

#include <string>
#include "DefaultMacros.hpp"

#ifdef _WIN32
    #include <windows.h>
    using LibraryHandle = HMODULE;
#else
    #include <dlfcn.h>
    using LibraryHandle = void*;
#endif

ELIX_NAMESPACE_BEGIN

namespace LibrariesLoader
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

ELIX_NAMESPACE_END

#endif //SCRIPTS_LOADER_HPP


