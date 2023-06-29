#include "include/UniqueWordsCounter.hpp"
#include "include/defaults.hpp"
// #include "include/helpers.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_set>
#include <execution>
#include <chrono>

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
    }


    auto word = std::string{};
    
    // VECTOR VARIANT AND SORT
    // {
    //     std::ifstream ifs(argv[1], std::ifstream::in);
    //     const auto start = std::chrono::steady_clock::now();

    //     auto uniqueWords_vector = std::vector<std::string>{};
    //     while (ifs >> word)

    //     {
    //         uniqueWords_vector.push_back(word);
    //     }
        
    //     const auto sort_start = std::chrono::steady_clock::now();
    //     sort(uniqueWords_vector.begin(), uniqueWords_vector.end());
    //     const auto sort_end = std::chrono::steady_clock::now();
        
    //     auto last = std::unique(uniqueWords_vector.begin(), uniqueWords_vector.end());


    //     const auto finish = std::chrono::steady_clock::now();
        
    //     std::cout << "unique_counter: " << last - uniqueWords_vector.begin();
    //     std::cout << " | uniqueWords_vector time: "
    //         << (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)).count() << " ms";
    //     std::cout << " | sort time: "
    //         << (std::chrono::duration_cast<std::chrono::milliseconds>(sort_end - sort_start)).count() << " ms" << "\n" << std::endl;
    // }
    // ---------------------- // 


    // VECTOR VARIANT AND PAREALLEL SORT
    // {
    //     std::ifstream ifs(argv[1], std::ifstream::in);
    //     const auto start = std::chrono::steady_clock::now();

    //     auto uniqueWords_vector_par = std::vector<std::string>{};
    //     while (ifs >> word)
    //     {
    //         uniqueWords_vector_par.push_back(word);
    //     }
        
    //     const auto sort_start = std::chrono::steady_clock::now();
    //     sort(std::execution::par, uniqueWords_vector_par.begin(), uniqueWords_vector_par.end());
    //     const auto sort_end = std::chrono::steady_clock::now();

    //     auto last1 = std::unique(uniqueWords_vector_par.begin(), uniqueWords_vector_par.end());

    //     const auto finish = std::chrono::steady_clock::now();

    //     std::cout << "unique_counter: " << last1 - uniqueWords_vector_par.begin();
    //     std::cout << " | uniqueWords_vector_par time: "
    //         << (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)).count() << " ms";
    //     std::cout << " | sort time: "
    //         << (std::chrono::duration_cast<std::chrono::milliseconds>(sort_end - sort_start)).count() << " ms\n" << std::endl;
    // }
    // ---------------------- //    


    // UNORDERED_MAP WITHOUT REHASH 
    // {
    //     std::ifstream ifs(argv[1], std::ifstream::in);
    //     const auto start2 = std::chrono::steady_clock::now();

    //     auto uniqueWords_hashmap = std::unordered_set<std::string>{};

    //     while (ifs >> word)
    //     {
    //         // std::cout << "word: " << word << std::endl;
    //         uniqueWords_hashmap.insert(std::move(word));
    //     }

    //     const auto finish2 = std::chrono::steady_clock::now();

    //     std::cout << "unique_counter: " << uniqueWords_hashmap.size();
    //     std::cout << " uniqueWords_hashmap TIME: "
    //         << (std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - start2)).count() << " ms\n" << std::endl;
    // }
    // ----------------------------------------/ 


    // UNORDERED_MAP WITH REHASH 
    {
        std::ifstream ifs(argv[1], std::ifstream::in);
        const auto start3 = std::chrono::steady_clock::now();

        auto uniqueWords_hashmap_rehash = std::unordered_set<std::string>{};

        uniqueWords_hashmap_rehash.rehash(10000000);
        while (ifs >> word)
        {
            // std::cout << "word: " << word << std::endl;
            uniqueWords_hashmap_rehash.insert(std::move(word));
        }

        const auto finish3 = std::chrono::steady_clock::now();

        std::cout << "unique_counter: " << uniqueWords_hashmap_rehash.size();
        std::cout << " uniqueWords_hashmap_rehash TIME: " 
            << (std::chrono::duration_cast<std::chrono::milliseconds>(finish3 - start3)).count() << " ms\n" << std::endl;
    }
    // ----------------------------------------/ 

    // UniqueWordsCounter counter(kFileName, FileReadMethod::kMmapMethod);
    // int32_t unique_words = counter.Count();


    return 0;
}