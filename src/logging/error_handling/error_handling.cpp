#include <cstdlib>
#include <iostream>

#include "error_handling.hpp"

#include "logging/logger/logger.hpp"

void logAndExit(const char *message) {
    Logger::instance().logCrit(message);
    Logger::instance().flush();
    std::exit(1);
}
