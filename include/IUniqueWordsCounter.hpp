#pragma once

#include "defaults.hpp"

#include <stdint.h>
#include <memory>
#include <iostream>

class ParallelIfstreamRecursive;

class IUniqueWordsCounter {
    public:
        virtual ~IUniqueWordsCounter() {};

        virtual uint32_t Count() = 0;
        static std::unique_ptr<IUniqueWordsCounter> CreateCounter(const std::string& filename, CounterType counter_type);
};
