#include "include/UniqueWordsCounter.hpp"
#include "include/defaults.hpp"
#include "include/basic_ideas.hpp"

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

    // simple parsing start
    if (argc != 2) {
        std::cout << "incorrect num of argv" << std::endl;
        return -1;
    }

    const std::string kFileName = argv[1];

    if (!std::filesystem::exists(kFileName)) {
        std::cout << "file " << kFileName << " doesn't exist" << std::endl;
        return 0;
    }

    // vector_sort_variant_1_thread(kFileName);
    // unord_set_no_reserve_1_thread(kFileName);
    // unord_set_with_reserve_1_thread(kFileName); // 17.7GB - 571017ms
    // mmap_unord_set_with_reserve_1_thread(kFileName);
    // paralell_ifstream_v1(kFileName); // killed by system if 17.7GB. because used std::string to save info from ifstream
    paralell_ifstream_v2(kFileName); // 17.7 GB - 123603ms
    // paralell_ifstream_v3(kFileName); // killed by system if 17.7GB. because used std::string to save info from maped file

    // UniqueWordsCounter counter(kFileName, FileReadMethod::kMmapMethod);
    // int32_t unique_words = counter.Count();

    return 0;
}
