#include "../include/ParallelIfstreamRecursive.hpp"
#include "../include/helpers.hpp"
#include "../include/defaults.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <future>

std::unordered_set<std::string> RecoursiveRountine(const std::string& filename, uint64_t start, uint64_t end, uint64_t chunk_length) {
    const uint64_t kLength = end - start;
    if (kLength <= chunk_length) {
        std::ifstream ifs(filename.c_str(), std::ifstream::in);
        ifs.seekg(start);
        std::unordered_set<std::string> unique_words;
        unique_words.rehash(kMaxUniqueWords);
        std::string word;
        int length_count = 0;
        while (ifs >> word) {
            unique_words.insert(std::move(word));
            length_count+=word.size();
            if (length_count >= kLength) break;
        }

        return std::move(unique_words);
    }

    const uint64_t kMid = start + kLength / 2;
    std::future<std::unordered_set<std::string>> fut = std::async(std::launch::deferred | std::launch::async,
                                                            RecoursiveRountine,
                                                            filename,
                                                            kMid,
                                                            end,
                                                            chunk_length);
    auto unique_words = RecoursiveRountine(filename, start, kMid, chunk_length);
    unique_words.rehash(2200000);
    auto future_unique_res = fut.get();
    unique_words.insert(future_unique_res.begin(), future_unique_res.end());
    return std::move(unique_words);
}

ParallelIfstreamRecursive::ParallelIfstreamRecursive(const std::string& filename) : filename_(filename) {}

ParallelIfstreamRecursive::~ParallelIfstreamRecursive() {}

uint32_t ParallelIfstreamRecursive::Count() {
    uint64_t end_position = 0;
    {
        std::ifstream ifs(filename_, std::ifstream::in);
        ifs.seekg(0, std::ios::end);
        end_position = ifs.tellg();
    }
    
    uint64_t current_chunk_length = 0;
    if (end_position < kMinChunkLength) {
        current_chunk_length = end_position;
    } else {
        const uint32_t kNumOfThreadsWithMinChunk = end_position / kMinChunkLength;
        const uint32_t kNumOfThreadsToUse = std::min(kNumOfThreadsWithMinChunk, kAllowedThreadsByHardware);

        // +1 below is quick fix to reduce сreating extra threads because of several calculation of the middle
        current_chunk_length = end_position / kNumOfThreadsToUse + 1;
    }

    auto final_res = RecoursiveRountine(filename_, 0, end_position, current_chunk_length);
    return static_cast<uint32_t>(final_res.size());
}
