#include "mpatcher.hpp"

#include "logger.hpp"

#include <Windows.h>
#include <cstring>

namespace utils 
{
    namespace mpatcher
    {
        void Patch(const uintptr_t address, const char* newData, size_t originalSize)
        {
            if (sizeof(newData) > originalSize) {
                logger.error("[mpatcher.cpp] ERROR Couldn't patch at 0x%d: new data is too long", address);
                return;
            }

            // Do patch
            DWORD oldProtect;
            if (VirtualProtect((void*)address, originalSize, PAGE_READWRITE, &oldProtect)) 
            {
                memcpy((void*)address, newData, originalSize);
                // Restore memory protection
                VirtualProtect((void*)address, originalSize, oldProtect, &oldProtect);

                logger.info("[mpatcher.cpp] Patched at 0x%d with data %s", address, newData);
            }
        }
    }
}