#include <string>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

//! A logger that writes data to file and to cout in paralel
class Logger {

public:
    inline static const std::string LOG_FILE_NAME = "gaem.log";
    inline static const std::string FILE_LOGGER_NAME = "flog";
    inline static const std::string CONSOLE_LOGGER_NAME = "clog";

    //! Turn on the debug level(by default is info, if true => debug, false =>
    //! back to info)
    static void setDebugMode(bool value) {
        if (value) {
            spdlog::set_level(spdlog::level::debug);
        } else {
            spdlog::set_level(spdlog::level::info);
        }
    }

    static Logger &instance() {
        static Logger l;
        return l;
    }
    //! Initialize the logger, MUST BE CALLED before the first usage. Deletes
    //! all log file if it exists And creates console and file loggers
    void init();

    Logger(const Logger &other) = delete;
    Logger &operator=(const Logger &other) = delete;

    //! Log debug info, logged ONLY if debug is enabled. Logs only to console
    void logDebug(const std::string &msg);
    //! Log info, logs both file and console
    void logInfo(const std::string &msg);
    //! Log warning, logs both file and console
    void logWarn(const std::string &msg);
    //! Log error, logs both file and console
    void logErr(const std::string &msg);
    //! Log critical, logs both file and console
    void logCrit(const std::string &msg);

    //! Flush all the logger data to file - should be called at crash so that
    //! the info is saved
    void flush() { m_fileLogger->flush(); }

private:
    Logger() {}

    std::shared_ptr<spdlog::logger> m_consoleLogger;
    std::shared_ptr<spdlog::logger> m_fileLogger;
};
