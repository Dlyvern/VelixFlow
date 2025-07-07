#include "LibrariesLoader.hpp"
#include <iostream>
#include "Logger.hpp"

ELIX_NAMESPACE_BEGIN

LibraryHandle LibrariesLoader::loadLibrary(const std::string &libraryPath)
{
#ifdef _WIN32
    HMODULE lib = LoadLibrary(libraryPath.c_str());

    if (!lib)
        ELIX_LOG_ERROR("Failed to load library ");

    return lib;
#else

    void* handle = dlopen(libraryPath.c_str(), RTLD_LAZY);

    if (!handle)
        ELIX_LOG_ERROR("Failed to load library ", dlerror());

    return handle;
#endif
}

void* LibrariesLoader::getFunction(const std::string &functionName, LibraryHandle library)
{
#ifdef _WIN32
    FARPROC function = GetProcAddress(library, functionName.c_str());

    if (!function)
        ELIX_LOG_ERROR("Failed to get function");

    return reinterpret_cast<void*>(function);
#else
 
    void* function = dlsym(library, functionName.c_str());

    if (!function)
        ELIX_LOG_ERROR("Failed to get function ", dlerror());

    return function;

#endif
}

void LibrariesLoader::closeLibrary(LibraryHandle library)
{
#ifdef _WIN32
    FreeLibrary(library);
#else
    dlclose(library);
#endif
}

ELIX_NAMESPACE_END