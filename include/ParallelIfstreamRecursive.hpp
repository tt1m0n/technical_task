#pragma once

#include "IUniqueWordsCounter.hpp"

#include <string>

class ParallelIfstreamRecursive : public IUniqueWordsCounter {
    public:
        ParallelIfstreamRecursive(const std::string& filename);
        ~ParallelIfstreamRecursive();
        uint32_t Count() override;

    private:
        std::string filename_;
        const char *file_head_;
        int fd_;
};