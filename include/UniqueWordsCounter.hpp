#pragma once 
#include "defaults.hpp"
#include "IFileHandler.hpp"

#include <string>
#include <memory>

class UniqueWordsCounter {
    public:
        UniqueWordsCounter() = delete;
        UniqueWordsCounter(const std::string& filename, FileReadMethod read_file_method);

        /**
         * @brief Count Unique words in the file
         * @return If success return count of unique words otherwise -1
         */
        int32_t Count();

    private:
        /**
         * @brief     Fabric method for creating file handler
         * 
         * @param[in] filename - name of he file need to handle
         * @param[in] read_file_method - enum with read file method (mmap, stream buffers, any other methods, etc)
         * 
         * @return unique_ptr to Object of FileHandler
         */
        std::unique_ptr<IFileHandler> CreateFileHandler(const std::string& filename, FileReadMethod read_file_method) const;


    private:
        std::unique_ptr<IFileHandler> file_handler_; 
};