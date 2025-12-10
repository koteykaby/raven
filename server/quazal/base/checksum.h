// PRUDP packet checksum implementation
#pragma once

#include <cstdint>
#include <cstring>

namespace Quazal
{
    uint8_t get_protocol_setting(uint8_t protocol);
    uint8_t make_checksum(const uint8_t* data, size_t length, uint8_t setting);
}