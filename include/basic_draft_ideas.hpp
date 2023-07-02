
/**
 * In this file I have collected basic draft ideas. It just my thoughts, even some of them do not use parallel computing.
 * Each of this variant can be research more deeply and can be improved.
 */

/**to call draft in mains. copy this code.

    // drafts
    // vector_sort_variant_1_thread(kFileName);
    // unord_set_no_reserve_1_thread(kFileName);
    // unord_set_with_reserve_1_thread(kFileName); // 17.7GB - 571017ms
    // mmap_unord_set_with_reserve_1_thread(kFileName);
    // paralell_ifstream_v1(kFileName); // killed by system if 17.7GB. because used std::string to save info from ifstream
*/

#include "defaults.hpp"
#include "helpers.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <future>
#include <unordered_set>

// mmap and helper need review and delete extra headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std::chrono;

void print_res(const std::string& method,
    int32_t unique_words,
    steady_clock::time_point start,
    steady_clock::time_point end)
{
    std::cout << method << ": unique - " << unique_words;
    std::cout << " | time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() << " ms" << std::endl;;
}

/**
 * @brief       ifstream & vector & sort & unique
 * @description The idea is:
 *              1) read worb by word from stream and save words to vector
 *              2) std::sort. We can use std::execution::par (for this purpose needed library tbb-thread building blocks > 2018v.
 *                 In my Ubuntu 18.04 i couldn't install this library. 
 *              3) use std::unique to delete repeated words
 */
void vector_sort_variant_1_thread(const std::string& filename)
{
    //start timer
    const auto start = std::chrono::steady_clock::now();

    std::ifstream ifs(filename, std::ifstream::in);
    auto unique_words = std::vector<std::string>{};
    std::string word;
    while (ifs >> word)
    {
        unique_words.push_back(word);
    }
    
    const auto sort_start = std::chrono::steady_clock::now();
    sort(unique_words.begin(), unique_words.end());
    const auto sort_end = std::chrono::steady_clock::now();
    
    auto last = std::unique(unique_words.begin(), unique_words.end());

    // end timer
    const auto finish = std::chrono::steady_clock::now();

    print_res("vector_sort_variant_1_thread", last - unique_words.begin(), start, finish);
}

/**
 * @brief       ifstream & unordered_set
 * @description The idea is:
 *              1) read word by word from stream and save words to unordered_set
 *              2) return size of unordered_set
 */
void unord_set_no_reserve_1_thread(const std::string& filename)
{
    //start timer
    const auto start = std::chrono::steady_clock::now();

    std::ifstream ifs(filename, std::ifstream::in);
    auto unique_words = std::unordered_set<std::string>{};
    std::string word;
    while (ifs >> word)
    {
        unique_words.insert(std::move(word));
    }

    // end timer
    const auto finish = std::chrono::steady_clock::now();

    print_res("unord_set_no_reserve_1_thread",unique_words.size(), start, finish);
}

/**
 * @brief       ifstream & unordered_set
 * @description The idea is:
 *              1) get fileLength and reserve unordered_set
 *              1) read word by word from stream and save words to unordered_set
 *              2) return size of unordered_set
 */
void unord_set_with_reserve_1_thread(const std::string& filename)
{
    //start timer
    const auto start = std::chrono::steady_clock::now();
    std::ifstream ifs(filename, std::ifstream::in);

    auto unique_words = std::unordered_set<std::string>{};
    unique_words.reserve(kMaxUniqueWords);
    std::string word;
    while (ifs >> word)
    {
        unique_words.insert(std::move(word));
    }

    // end timer
    const auto finish = std::chrono::steady_clock::now();

    print_res("unord_set_with_reserve_1_thread", unique_words.size(), start, finish);
}

/**
 * @brief       mmap & unordered_set
 * @description The idea is:
 *              1) map file
 *              1) read word by word from stream and save words to unordered_set
 *              2) return size of unordered_set
 */
void mmap_unord_set_with_reserve_1_thread(const std::string& filename)
{
    // start timer
    const auto start = std::chrono::steady_clock::now();

    struct stat sb;
    int fd_ = open(filename.c_str(), O_RDONLY);
    fstat(fd_, &sb);
    const size_t kFileSize = static_cast<size_t>(sb.st_size);

    const char* kFileHead = static_cast<const char*> (mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd_, 0));
    if (kFileHead == MAP_FAILED) {
        return;
    }

    auto uniqueWords = std::unordered_set<std::string>{};
    uniqueWords.reserve(kMaxUniqueWords);

    {
        std::istringstream ss(std::string(kFileHead, kFileSize));
        std::string word;
        while (ss >> word) {
            uniqueWords.insert(std::move(word));
        }
    }

    // end timer
    const auto finish = std::chrono::steady_clock::now();

    print_res("mmap_unord_set_with_reserve_1_thread", uniqueWords.size(), start, finish);
}

/**
 * @brief       2 Functions Below [V1] uses: ifstreams, chunks, threads, unordered_maps
 * @description The idea is:
 *              1) Split file into chunks. chunk_size = file_size / kMaxAllowThreadsOnTheSystem
 *              2) for each chunk make recursive call std::async:
 *                 2.1) open ifstream
 *                 2.2) read chunk_size content into string and close ifstream
 *                 2.3) split string to words using istringstream and save into unordered_set
 *              3) merged all unordered_sets from different threads
 *              4) return size
 */
std::unordered_set<std::string> parallel_recursive_routine_v1(const std::string& filename, uint64_t start, uint64_t end, uint64_t chunk_length) {
    const uint64_t kLength = end - start;
    if (kLength <= chunk_length) {
        // std::cout << std::this_thread::get_id() << std::endl;

        std::string contents;
        contents.resize(kLength);
        {
            std::ifstream ifs(filename.c_str(), std::ifstream::in);
            ifs.seekg(start);
            ifs.read(&contents[0], kLength);
        }

        std::unordered_set<std::string> unique_words;
        std::istringstream ss(contents);
        std::string word;
        while (ss >> word) {
            unique_words.insert(std::move(word));
        }

        return std::move(unique_words);
    }

    const uint64_t kMid = start + kLength / 2;
    std::future<std::unordered_set<std::string>> fut = std::async(std::launch::deferred | std::launch::async,
                                                            parallel_recursive_routine_v1,
                                                            filename,
                                                            kMid,
                                                            end,
                                                            chunk_length);
    auto unique_words = parallel_recursive_routine_v1(filename, start, kMid, chunk_length);
    auto future_unique_res = fut.get();
    unique_words.insert(future_unique_res.begin(), future_unique_res.end());
    return unique_words;
}

void paralell_ifstream_v1(const std::string& filename)
{
    // start timer
    const auto start = std::chrono::steady_clock::now();

    uint64_t end_position = 0;
    {
        std::ifstream ifs(filename, std::ifstream::in);
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

    auto final_res = parallel_recursive_routine_v1(filename, 0, end_position, current_chunk_length);

    // end timer
    const auto end = std::chrono::steady_clock::now();

    print_res("paralell_ifstream_v1", final_res.size(), start, end);

}