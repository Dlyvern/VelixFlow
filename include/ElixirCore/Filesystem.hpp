#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <filesystem>
#include <string>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

#include "DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

namespace filesystem
{
    inline std::filesystem::path getExecutablePath()
    {
    #if defined(_WIN32)
        char buffer[MAX_PATH];
        DWORD size = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        if (size == 0 || size == MAX_PATH)
            return {};
        return std::filesystem::path(buffer).parent_path();

    #elif defined(__linux__)
        char buffer[1024];
        ssize_t size = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (size <= 0 || size >= static_cast<ssize_t>(sizeof(buffer)))
            return {};
        return std::filesystem::path(std::string(buffer, size)).parent_path();

    #elif defined(__APPLE__)
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0)
            return {};
        return std::filesystem::path(buffer).parent_path();

    #else
        return {};
    #endif
    }

    inline std::filesystem::path getCurrentWorkingDirectory()
    {
        return std::filesystem::current_path();
    }

    inline std::filesystem::path getResourcesFolderPath()
    {
        std::string path = getCurrentWorkingDirectory().string();

        const size_t position = path.find_last_of('/');

        path = path.substr(0, position);

        path += "/resources";

        return {path};
    }
}

ELIX_NAMESPACE_END

#endif //FILESYSTEM_HPP