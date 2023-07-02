#pragma once

#include "IUniqueWordsCounter.hpp"
#include <string>
#include <unordered_set>

/**
 * @brief       This count method uses: ifstreams, chunks, threads, unordered_maps
 * @description The idea is:
 *              1) Split file into chunks.
 *              2) for each chunk make call std::async:
 *                 2.1) open ifstream
 *                 2.2) read word by word directly from the stream until until sum of characters >= lenght.
 *                 2.3) save into unordered_set
 *              3) merged all unordered_sets from different threads
 *              4) return size
 */

class ParallelIfstreamRecursive : public IUniqueWordsCounter {
    public:
        ParallelIfstreamRecursive(const std::string& filename);
        ~ParallelIfstreamRecursive();
        uint32_t Count() override;

    private:
        std::string filename_;
};