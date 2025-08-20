// Simple .rdata section patcher
#include "logger.hpp"
#include <cstring>

namespace utils
{
    namespace mpatcher
    {
        void Patch(const uintptr_t address, const char* newData, size_t originalSize);
    }
}