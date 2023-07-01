#pragma once

#include <iostream>
#include <thread>

const std::string kTypeError("error: ");
const std::string kTypeInfo("info: ");

const uint32_t kAllowedThreadsByHardware = std::thread::hardware_concurrency();
const uint32_t kMinChunkLength = 10 * 1024; // 10MB

enum class FileReadMethod {
    kMmapMethod = 0,
};

enum class CounterType {
    kParalellIfstreamRecursive = 0,
    kParalellMmapRecursive,
};