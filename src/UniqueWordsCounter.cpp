#include "../include/UniqueWordsCounter.hpp"
#include "../include/MmapFileHandler.hpp"
#include "../include/helpers.hpp"
#include "../include/defaults.hpp"

#include <iostream>

UniqueWordsCounter::UniqueWordsCounter(const std::string& filename, FileReadMethod read_file_method) {
        file_handler_ = CreateFileHandler(filename, read_file_method);
    }

int32_t UniqueWordsCounter::Count() {
    if (file_handler_ == nullptr) {
        log_message(kTypeError, __FILE__, __LINE__, "file handler doesn't exist");
        return -1;
    }

    const uint32_t kUniqueWords = file_handler_->CountUniqueWords();
    return kUniqueWords;
}

std::unique_ptr<IFileHandler> UniqueWordsCounter::CreateFileHandler(const std::string& filename,
    FileReadMethod read_file_method) const {
    switch(read_file_method) {
        case FileReadMethod::kMmapMethod :
            return std::make_unique<MmapFileHandler>(filename);
            break;
        
        default :
            break;
    }

    return nullptr;
}