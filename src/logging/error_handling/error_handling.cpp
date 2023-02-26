#include <cstdlib>
#include <iostream>

#include "error_handling.hpp"

void printAndExit(const char *message) {
    std::cerr << "FATAL: " << message << std::endl;
    std::exit(1);
}
