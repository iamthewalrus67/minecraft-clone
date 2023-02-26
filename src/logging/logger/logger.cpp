#include <filesystem>

#include "logger.hpp"

void Logger::init() {
    // Clear file at each run - delete it basically
    if (std::filesystem::exists(LOG_FILE_NAME)) {
        std::filesystem::remove(LOG_FILE_NAME);
    }

    m_consoleLogger = spdlog::stdout_color_mt(CONSOLE_LOGGER_NAME);
    m_fileLogger = spdlog::basic_logger_mt(FILE_LOGGER_NAME, LOG_FILE_NAME);
}

void Logger::logDebug(const std::string &msg) { m_consoleLogger->debug(msg); }
void Logger::logInfo(const std::string &msg) {
    m_consoleLogger->info(msg);
    m_fileLogger->info(msg);
}
void Logger::logWarn(const std::string &msg) {
    m_consoleLogger->warn(msg);
    m_fileLogger->warn(msg);
}
void Logger::logErr(const std::string &msg) {
    m_consoleLogger->error(msg);
    m_fileLogger->error(msg);
}
void Logger::logCrit(const std::string &msg) {
    m_consoleLogger->critical(msg);
    m_fileLogger->critical(msg);
}
