#include "Logger.h"
#include "Utils.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <filesystem>
#include <iostream>

namespace Ralph {

void Logger::Init() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    std::string logPath = Utils::GetSavePath("latest.log");
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logPath, 1024 * 1024 * 5, 3);
    file_sink->set_level(spdlog::level::info);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("RalphArcade", sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);

    LOG_INFO("Logger inicializado com sucesso!");
    LOG_INFO("Arquivo de log: {}", logPath);
}

void Logger::Shutdown() {
    spdlog::shutdown();
}

} // namespace Ralph
