#include "LibrariesLoader.hpp"

#include <iostream>

LibraryHandle elix::LibrariesLoader::loadLibrary(const std::string &libraryPath)
{
#ifdef _WIN32
    HMODULE lib = LoadLibrary(libraryPath.c_str());

    if (!lib)
        std::cerr << "LibrariesLoader::loadLibrary(): Failed to load library "<< libraryPath << std::endl;

    return lib;
#else

    void* handle = dlopen(libraryPath.c_str(), RTLD_LAZY);

    if (!handle)
        std::cerr << "LibrariesLoader::loadLibrary(): Failed to load library "<< libraryPath << " " << dlerror() << std::endl;

    return handle;
#endif
}

void* elix::LibrariesLoader::getFunction(const std::string &functionName, LibraryHandle library)
{
#ifdef _WIN32
    FARPROC function = GetProcAddress(library, functionName.c_str());

    if (!function)
        std::cerr << "LibrariesLoader::getFunction(): Failed to get function " << functionName << std::endl;

return reinterpret_cast<void*>(function);
#else

    void* function = dlsym(library, functionName.c_str());

    if (!function)
        std::cerr << "LibrariesLoader::getFunction(): Failed to get function " << functionName << " " << dlerror() << std::endl;

    return function;

#endif
}

void elix::LibrariesLoader::closeLibrary(LibraryHandle library)
{
#ifdef _WIN32
    FreeLibrary(library);
#else
    dlclose(library);
#endif
}
