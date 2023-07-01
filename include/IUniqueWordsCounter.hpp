#pragma once

#include "defaults.hpp"
#include "ParallelIfstreamRecursive.hpp"

#include <stdint.h>
#include <memory>

class ParallelIfstreamRecursive;

class IUniqueWordsCounter {
    public:
        virtual ~IUniqueWordsCounter() {};
        virtual uint32_t Count() = 0;

        static std::unique_ptr<IUniqueWordsCounter> CreateCounter(CounterType counter_type) {
            switch (counter_type) {
                case CounterType::kParalellIfstreamRecursive {
                    return make_unique(new ParallelIfstreamRecursive);
                }
            }
};