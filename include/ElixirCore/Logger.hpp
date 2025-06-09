#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <sstream>
#include <type_traits>
#include <utility>

#if defined(_WIN32)
#include <windows.h>
#endif


#define ELIX_LOG_INFO(...) elix::Logger::log(elix::Logger::LogLevel::ELIX_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ELIX_LOG_WARN(...) elix::Logger::log(elix::Logger::LogLevel::ELIX_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define ELIX_LOG_ERROR(...) elix::Logger::log(elix::Logger::LogLevel::ELIX_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

namespace elix
{
    class Logger
    {
    public:
        enum class LogLevel
        {
            ELIX_INFO,
            ELIX_WARNING,
            ELIX_ERROR
        };

        struct Message
        {
            std::string message;
            glm::vec3 color;
            std::chrono::system_clock::time_point timestamp;
        };


        static Logger& instance();

        static const char* logLevelToString(LogLevel level)
        {
            switch(level)
            {
                case LogLevel::ELIX_INFO: return "INFO";
                case LogLevel::ELIX_WARNING: return "WARNING";
                case LogLevel::ELIX_ERROR: return "ERROR";
                default: return "UNKNOWN";
            }
        }

        template<typename... Args>
        static void log(LogLevel level, const char* file, int line, const char* function, Args&&... args)
        {
            std::ostringstream oss;
            (instance().streamArg(oss, std::forward<Args>(args)), ...);
            instance().logInternal(level, oss.str(), file, line, function);
        }

        [[nodiscard]] const std::vector<Message>& getMessages() const;

    private:
        std::mutex m_mutex;

        std::vector<Message> m_messages;

#if defined(_WIN32)
        HANDLE m_consoleHandle = nullptr;
        bool m_consoleColorSupported = false;
#endif

        template<typename T>
        void streamArg(std::ostringstream& oss, T&& arg)
        {
            if constexpr (std::is_enum_v<std::decay_t<T>>)
                oss << static_cast<std::underlying_type_t<std::decay_t<T>>>(arg);
            else
                oss << std::forward<T>(arg);
        }

        void setConsoleColor(LogLevel level);
        void resetConsoleColor();

        void logInternal(LogLevel level, const std::string& message, const char* file, int line, const char* function);

        std::string formatMessage(LogLevel level, const std::string& message, const char* file = nullptr, int line = -1, const char* function = nullptr);
        std::string getTimestamp();
        Logger();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    };
} //namespace elix
#endif //LOGGER_HPP
