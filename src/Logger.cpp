#include "Logger.hpp"

#include <cstring>
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

elix::Logger::Logger()
{
#if defined(_WIN32)
    m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (m_consoleHandle != INVALID_HANDLE_VALUE)
    {
        DWORD consoleMode;
        m_consoleColorSupported = GetConsoleMode(m_consoleHandle, &consoleMode) &&
                                 SetConsoleMode(m_consoleHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

void elix::Logger::setConsoleColor(LogLevel level)
{
#if defined(_WIN32)
    if (m_consoleColorSupported)
    {
        // Using ANSI escape codes if virtual terminal processing is supported
        switch(level)
        {
            case LogLevel::ELIX_INFO: std::cout << "\033[32m"; break;    // Green
            case LogLevel::ELIX_WARNING: std::cout << "\033[33m"; break; // Yellow
            case LogLevel::ELIX_ERROR: std::cout << "\033[31m"; break;    // Red
        }
    }
    else if (m_consoleHandle != INVALID_HANDLE_VALUE)
    {
        // Fall back to Windows console API if ANSI not supported
        WORD color = 0;
        switch(level)
        {
            case LogLevel::ELIX_INFO: color = FOREGROUND_GREEN; break;
            case LogLevel::ELIX_WARNING: color = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case LogLevel::ELIX_ERROR: color = FOREGROUND_RED; break;
        }
        SetConsoleTextAttribute(m_consoleHandle, color);
    }
#else
    // Linux/macOS uses ANSI escape codes
    switch(level)
    {
        case LogLevel::ELIX_INFO: std::cout << "\033[32m"; break;    // Green
        case LogLevel::ELIX_WARNING: std::cout << "\033[33m"; break; // Yellow
        case LogLevel::ELIX_ERROR: std::cout << "\033[31m"; break;    // Red
    }
#endif
}

void elix::Logger::resetConsoleColor()
{
#if defined(_WIN32)
    if (m_consoleColorSupported)
    {
        std::cout << "\033[0m";
    }
    else if (m_consoleHandle != INVALID_HANDLE_VALUE)
    {
        SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
#else
    std::cout << "\033[0m";
#endif
}


elix::Logger& elix::Logger::instance()
{
    static elix::Logger logger;
    return logger;
}

const std::vector<elix::Logger::Message>& elix::Logger::getMessages() const
{
    return m_messages;
}

void elix::Logger::logInternal(LogLevel level, const std::string &message, const char *file, int line, const char *function)
{
    std::string formatted = formatMessage(level, message, file, line, function);

    //TODO write shit to terminal
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        setConsoleColor(level);

        (level == LogLevel::ELIX_INFO ? std::cout : std::cerr) << formatted << std::endl;

        resetConsoleColor();
    }

    // Thread-safe message storage
    glm::vec3 color;

    switch(level)
    {
        case LogLevel::ELIX_INFO: color = {0.8f, 0.8f, 0.8f}; break;
        case LogLevel::ELIX_WARNING: color = {1.0f, 0.8f, 0.0f}; break;
        case LogLevel::ELIX_ERROR: color = {1.0f, 0.3f, 0.3f}; break;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        Message msg;

        msg.color = color;
        msg.message = message;
        msg.timestamp = std::chrono::system_clock::now();

        m_messages.push_back(msg);
        if (m_messages.size() > 1000) {
            m_messages.erase(m_messages.begin());
        }
    }
}

std::string elix::Logger::formatMessage(LogLevel level, const std::string &message, const char* file, int line, const char* function)
{
    std::stringstream ss;

    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    const auto timer = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &timer);
#else
    localtime_r(&timer, &tm);
#endif

    ss << "[" << std::put_time(&tm, "%H:%M:%S.")
       << std::setfill('0') << std::setw(3) << ms.count() << "] ";

    switch (level)
    {
        case LogLevel::ELIX_INFO: ss << "[INFO]"; break;
        case LogLevel::ELIX_WARNING: ss << "[WARN]"; break;
        case LogLevel::ELIX_ERROR: ss << "[ERROR]"; break;
    }

    if (file && line > 0 && function)
    {
        const char* filename = strrchr(file, '/') ? strrchr(file, '/') + 1 :
                              (strrchr(file, '\\') ? strrchr(file, '\\') + 1 : file);
        ss << "[" << filename << ":" << line << "][" << function << "] ";
    }

    ss << message;

    return ss.str();
}

std::string elix::Logger::getTimestamp()
{
    std::time_t t = std::time(nullptr);

    // Thread-safe time formatting
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &tm);
    return std::string(buffer);
}


