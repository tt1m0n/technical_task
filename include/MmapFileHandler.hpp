#pragma once

#include "IFileHandler.hpp"

#include <string>

class MmapFileHandler : public IFileHandler {
    public:
        MmapFileHandler(const std::string& filename);
        ~MmapFileHandler();
        uint32_t CountUniqueWords() override;

    private:
        bool MapFile();

    private:
        std::string filename_;
        const char *file_head_;
        int fd_;
};