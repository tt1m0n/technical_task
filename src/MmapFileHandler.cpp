#include "../include/MmapFileHandler.hpp"
#include "../include/helpers.hpp"
#include "../include/defaults.hpp"

#include <string>
#include <iostream>

// mmap and helper need review and delete extra headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

MmapFileHandler::MmapFileHandler(const std::string& filename) : filename_(filename) {
}

MmapFileHandler::~MmapFileHandler() {}

uint32_t MmapFileHandler::CountUniqueWords() {
    if (MapFile() == false) {
        log_message(kTypeError, __FILE__, __LINE__, "map failed");
    }

    return 0;
}

bool MmapFileHandler::MapFile() {
    struct stat sb;

    fd_ = open(filename_.c_str(), O_RDONLY);
    fstat(fd_, &sb);

    // REMEMBER: MAP_PRIVATE is possible not correct way for large file
    file_head_ = static_cast<const char*> (mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd_, 0));
    if (file_head_ == MAP_FAILED) {
        return false;
    }

    log_message(kTypeInfo, __FILE__, __LINE__, "success mapped"); 
    return true;
}
