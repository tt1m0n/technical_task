#pragma once

#include "IUniqueWordsCounter.hpp"
#include <string>
#include <unordered_set>

/**
 * @brief       This count method uses: mmap, chunks, threads, unordered_maps
 * @description The idea is:
 *              1) Split file into chunks.
 *              2) for each chunk make call std::async:
 *                 2.1) read word by word directly from the stream until until sum of characters >= lenght.
 *                 2.2) save into unordered_set
 *              3) merged all unordered_sets from different threads
 *              4) return size
 */

class ParallelMmapRecursive : public IUniqueWordsCounter {
    public:
        ParallelMmapRecursive(const std::string& filename);
        ~ParallelMmapRecursive();
        uint32_t Count() override;

    private:
        std::string filename_;
        int fd_;
        const char* file_head_;
        size_t file_size_;
};