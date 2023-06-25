#pragma once

#include "IFileHandler.hpp"

class MmapFileHandler : public IFileHandler {
    public:
        uint32_t count_unique_words() override;
};