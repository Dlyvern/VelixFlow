#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <filesystem>
#include <string>
#include "DefaultMacros.hpp"
#include "Logger.hpp"

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif




ELIX_NAMESPACE_BEGIN

namespace filesystem
{
    //TODO maybe make this better....
    //TODO Wrap this inside try-catch?
    inline std::pair<int, std::string> executeCommand(const std::string& command)
    {
        constexpr int kBufferSize = 128;
        std::array<char, kBufferSize> buffer;
        std::string result;

#ifdef _WIN32
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
#else
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
#endif

        if (!pipe)
            ELIX_LOG_ERROR("Failed to execute command");

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            result += buffer.data();

        const int executionResult = std::system(command.c_str());

        return {executionResult, result};
    }

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

    inline void openInFileManager(const std::filesystem::path& path)
    {
        #ifdef _WIN32
            std::string cmd = "explorer /select,\"" + path.string() + "\"";
        #elif __APPLE__
                std::string cmd = "open -R \"" + path.string() + "\"";
        #else
                std::string cmd = "xdg-open \"" + path.parent_path().string() + "\"";
        #endif

        executeCommand(cmd);
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