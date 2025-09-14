#include "typeflags.hpp"

#include <cstdint>
#include <iostream>

namespace quazal
{
    namespace utils
    {
        PacketType ReadPacketType(uint8_t packetTypeFlagsByte)
        {
            return static_cast<PacketType>(packetTypeFlagsByte & 0x07); // 00000111
        }

        bool CheckFlag(uint8_t packetTypeFlagsByte, PacketFlags flag)
        {
            return (packetTypeFlagsByte & static_cast<uint8_t>(flag << 3)) != 0;
        }

        uint8_t CreateTypeFlagsByte(PacketType type, uint8_t flags = 0)
        {
            return (flags << 3) | (static_cast<uint8_t>(type) & 0x07);
        }
    }
}
