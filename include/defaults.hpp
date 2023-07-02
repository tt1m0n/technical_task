#pragma once

#include <iostream>
#include <thread>
#include <map>

const std::string kTypeError("error: ");
const std::string kTypeInfo("info: ");

const uint32_t kAllowedThreadsByHardware = std::thread::hardware_concurrency();
const uint32_t kMinChunkLength = 10 * 1024; // 10MB
const uint32_t kMaxUniqueWords = 2200000;

enum class FileReadMethod {
    kMmapMethod = 0,
};

enum class CounterType {
    kParalellIfstreamRecursive = 0,
    kParalellMmapRecursive,
};

const std::map<CounterType, std::string> kCounterTypeString {
    {CounterType::kParalellIfstreamRecursive, std::string("paralell ifstream recursive method")},
    {CounterType::kParalellMmapRecursive, std::string("paralell mmap recursive method")},
};