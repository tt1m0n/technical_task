#include "include/UniqueWordsCounter.hpp"
#include "include/defaults.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

int main(int argc, char **argv) {

    // -- parsing start
    if (argc != 2) {
        std::cout << "incorrect num of argv" << std::endl;
        return -1;
    }

    const std::string kFileName = argv[1];

    if (!std::filesystem::exists(kFileName)) {
        std::cout << "file " << kFileName << " doesn't exist" << std::endl;
    }

    // parsing end --
    
    UniqueWordsCounter counter(kFileName, FileReadMethod::kMmapMethod);
    int32_t unique_words = counter.Count();


    // mmap logic end --

    return 0;
}