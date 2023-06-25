#include "../include/UniqueWordsCounter.hpp"
#include "../include/MmapFileHandler.hpp"

#include <iostream>

// mmap and helper need review and delete extra headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

UniqueWordsCounter::UniqueWordsCounter(const std::string& filename, FileReadMethod read_file_method)
    : filename_(filename), read_file_method_(read_file_method) {
        file_handler_ = CreateFileHandler(read_file_method);
    }

uint32_t UniqueWordsCounter::Count() {
   const char *file_head;
   int fd;
   struct stat sb;

   fd = open(filename_.c_str(), O_RDONLY);
   fstat(fd, &sb);
   printf("Size: %lu\n", (uint64_t)sb.st_size);

   // REMEMBER MAP_PRIVATE is possible not correct way for large file
   file_head = static_cast<const char*> (mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
   if (file_head == MAP_FAILED) {
    std::cout << "ERROR" << std::endl;
   }

    std::cout << std::string(file_head) << std::endl;

    return 0;
}

std::unique_ptr<IFileHandler> UniqueWordsCounter::CreateFileHandler(FileReadMethod read_file_method) const {
    switch(read_file_method) {
        case FileReadMethod::kMmapMethod :
            return std::make_unique<MmapFileHandler>();
            break;
        
        default :
            break;
    }

    return nullptr;
}