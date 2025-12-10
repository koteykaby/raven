#pragma once
#include <cstdint>

namespace Quazal
{
    enum PacketFlags : uint8_t
    {
        ACK      = 1 << 0,
        RELIABLE = 1 << 1,
        NEED_ACK = 1 << 2,
        HAS_SIZE = 1 << 3
    };

    enum PacketType : uint8_t
    {
        SYN = 0,
        CONNECT = 1,
        DATA = 2,
        DISCONNECT = 3,
        PING = 4,
        USER = 5,
        ROUTE = 6,
        RAW = 7
    };

    inline uint8_t makeTypeFlags(PacketType type, uint8_t flags)
    {
        return (type & 0x07) | ((flags & 0x1F) << 3);
    }

    inline PacketType extractType(uint8_t raw)
    {
        return (PacketType)(raw & 0x07);
    }

    inline uint8_t extractFlags(uint8_t raw)
    {
        return (raw >> 3) & 0x1F;
    }
}
