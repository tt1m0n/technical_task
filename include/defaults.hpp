#pragma once

#include <iostream>

const std::string kTypeError("error: ");
const std::string kTypeInfo("info: ");

enum class FileReadMethod {
    kMmapMethod = 0,
};