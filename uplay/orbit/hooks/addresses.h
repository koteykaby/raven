#pragma once

#include <Windows.h>
#include <string>

#include <unordered_map>
#include <cstdint>

namespace hooks
{
    // Check what binary is attached for now
    const std::unordered_map<std::string, uintptr_t>* LoadBinaryAddresses();

    extern std::unordered_map<std::string, uintptr_t> addresses_dx9;
    extern std::unordered_map<std::string, uintptr_t> addresses_dx11;
}