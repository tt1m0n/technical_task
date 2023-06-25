#pragma once

 #include <stdint.h>

class IFileHandler {
    virtual uint32_t count_unique_words() = 0;
};