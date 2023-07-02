#include "../include/ParallelMmapRecursive.hpp"
#include "../include/helpers.hpp"
#include "../include/defaults.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <future>
#include <sstream>

// mmap and helpers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

std::unordered_set<std::string> RecoursiveRountine(const char* file_head, uint64_t start, uint64_t end, uint64_t chunk_length) {
    const uint64_t kLength = end - start;
    if (kLength <= chunk_length) {
        std::istringstream ss(std::string(file_head + start, end - start));
        std::string word;
        std::unordered_set<std::string> unique_words;
        while (ss >> word) {
            unique_words.insert(std::move(word));
        }

        return std::move(unique_words);
    }

    const uint64_t kMid = start + kLength / 2;
    std::future<std::unordered_set<std::string>> fut = std::async(std::launch::deferred | std::launch::async,
                                                            RecoursiveRountine,
                                                            file_head,
                                                            kMid + 1,
                                                            end,
                                                            chunk_length);
    auto unique_words = RecoursiveRountine(file_head, start, kMid, chunk_length);
    auto future_unique_res = fut.get();
    unique_words.insert(future_unique_res.begin(), future_unique_res.end());
    return std::move(unique_words);
}

ParallelMmapRecursive::ParallelMmapRecursive(const std::string& filename) : filename_(filename) {
    struct stat sb;
    int fd_ = open(filename.c_str(), O_RDONLY);
    fstat(fd_, &sb);
    file_size_ = static_cast<size_t>(sb.st_size);
    file_head_ = static_cast<const char*> (mmap(NULL, file_size_, PROT_READ, MAP_SHARED, fd_, 0));
}

ParallelMmapRecursive::~ParallelMmapRecursive() {
    close(fd_);
    munmap(const_cast<char*>(file_head_), file_size_);
}

uint32_t ParallelMmapRecursive::Count() {
    if (file_head_ == MAP_FAILED) return -1;

    uint64_t current_chunk_length = 0;
    if (file_size_ < kMinChunkLength) {
        current_chunk_length = file_size_;
    } else {
        const uint32_t kNumOfThreadsWithMinChunk = file_size_ / kMinChunkLength;
        const uint32_t kNumOfThreadsToUse = std::min(kNumOfThreadsWithMinChunk, kAllowedThreadsByHardware);

        // +1 below is quick fix to reduce сreating extra threads because of several calculation of the middle
        current_chunk_length = file_size_ / kNumOfThreadsToUse + 1;
    }

    auto final_res = RecoursiveRountine(file_head_, 0, file_size_, current_chunk_length);
    return static_cast<uint32_t>(final_res.size());
}
