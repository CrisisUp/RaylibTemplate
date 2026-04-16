#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <string>

namespace Ralph {

class Logger {
public:
    static void Init();
    static void Shutdown();
    
    // Get the main logger
    static std::shared_ptr<spdlog::logger> Get() { return spdlog::get("RalphArcade"); }
};

} // namespace Ralph

// Macros para facilitar o uso
#define LOG_TRACE(...) Ralph::Logger::Get()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Ralph::Logger::Get()->debug(__VA_ARGS__)
#define LOG_INFO(...)  Ralph::Logger::Get()->info(__VA_ARGS__)
#define LOG_WARN(...)  Ralph::Logger::Get()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Ralph::Logger::Get()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Ralph::Logger::Get()->critical(__VA_ARGS__)

#endif
