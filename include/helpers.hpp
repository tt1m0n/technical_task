#pragma once

#include <iostream>

static void log_message(const std::string& type, const std::string& file, int line, const std::string& msg) {
    std::cout << type << file << "(@" << line << "): " << msg << std::endl;
}