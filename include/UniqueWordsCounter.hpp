#pragma once 
#include "defaults.hpp"
#include "IFileHandler.hpp"

#include <string>
#include <memory>

class UniqueWordsCounter {
    public:
        UniqueWordsCounter() = delete;
        UniqueWordsCounter(const std::string& filename, FileReadMethod read_file_method);
        uint32_t Count();

    private:
        std::unique_ptr<IFileHandler> CreateFileHandler(FileReadMethod read_file_method) const;


    private:
        std::string filename_;
        FileReadMethod read_file_method_;
        std::unique_ptr<IFileHandler> file_handler_; 
};