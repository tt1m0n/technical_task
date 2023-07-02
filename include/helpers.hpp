#pragma once

#include "defaults.hpp"

#include <iostream>
#include <chrono>

using namespace std::chrono;

static void log_message(const std::string& type, const std::string& file, int line, const std::string& msg) {
    std::cout << type << file << "(@" << line << "): " << msg << std::endl;
}

static void show_results(CounterType type,
                        uint32_t unique_words,
                        steady_clock::time_point start,
                        steady_clock::time_point end) {
    std::cout << kCounterTypeString.at(type) << ": unique - " << unique_words;
    std::cout << " | time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() << " ms" << std::endl;;
}