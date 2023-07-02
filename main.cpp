#include "include/IUniqueWordsCounter.hpp"
#include "include/ParallelIfstreamRecursive.hpp"
#include "include/defaults.hpp"
#include "include/basic_draft_ideas.hpp"
#include "include/helpers.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <future>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "incorrect num of argv" << std::endl;
        return -1;
    }

    const std::string kFileName = argv[1];
    if (!std::filesystem::exists(kFileName)) {
        std::cout << "file " << kFileName << " doesn't exist" << std::endl;
        return -1;
    }

    std::vector<CounterType> counter_types{
        CounterType::kParalellIfstreamRecursive,
        // CounterType::kParalellMmapRecursive
    };

    for (auto type : counter_types) {
        const auto start = std::chrono::steady_clock::now();
        auto unique_words = IUniqueWordsCounter::CreateCounter(kFileName, type)->Count();
        const auto end = std::chrono::steady_clock::now();

        show_results(type, unique_words, start, end);
    }

    // drafts
    // vector_sort_variant_1_thread(kFileName);
    // unord_set_no_reserve_1_thread(kFileName);
    // unord_set_with_reserve_1_thread(kFileName); // 17.7GB - 571017ms
    // mmap_unord_set_with_reserve_1_thread(kFileName);
    // paralell_ifstream_v1(kFileName); // killed by system if 17.7GB. because used std::string to save info from ifstream

    return 0;
}
