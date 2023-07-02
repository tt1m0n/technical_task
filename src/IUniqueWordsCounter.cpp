#include "../include/IUniqueWordsCounter.hpp"
#include "../include/ParallelIfstreamRecursive.hpp"
#include "../include/ParallelMmapRecursive.hpp"

std::unique_ptr<IUniqueWordsCounter> IUniqueWordsCounter::CreateCounter(const std::string& filename, CounterType counter_type) {
    switch (counter_type) {
        case CounterType::kParalellIfstreamRecursive :
            return std::make_unique<ParallelIfstreamRecursive>(filename);
            break;

        case CounterType::kParalellMmapRecursive :
            return std::make_unique<ParallelMmapRecursive>(filename);
            break;
        default:
            return nullptr;
    }
}