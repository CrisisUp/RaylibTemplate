#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <string>

class Logger {
public:
    static void Init();
    static void Shutdown();
    
    // Get the main logger
    static std::shared_ptr<spdlog::logger> Get() { return spdlog::get("RalphArcade"); }
};

// Macros para facilitar o uso
#define LOG_TRACE(...) Logger::Get()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::Get()->debug(__VA_ARGS__)
#define LOG_INFO(...)  Logger::Get()->info(__VA_ARGS__)
#define LOG_WARN(...)  Logger::Get()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::Get()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::Get()->critical(__VA_ARGS__)

#endif
