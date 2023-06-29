#pragma once

#include <stdint.h>

class IFileHandler {
    public:
        virtual ~IFileHandler() {};
        virtual uint32_t CountUniqueWords() = 0;
};