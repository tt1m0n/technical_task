#include "../include/ParallelIfstreamRecursive.hpp"
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

ParallelIfstreamRecursive::ParallelIfstreamRecursive(const std::string& filename) : filename_(filename) {
}

ParallelIfstreamRecursive::~ParallelIfstreamRecursive() {}

uint32_t MmapFileHandler::CountUniqueWords() {
    if (MapFile() == false) {
        log_message(kTypeError, __FILE__, __LINE__, "map failed");
    }

    return 0;
}
