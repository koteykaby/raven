#pragma once

#include <cstdint>
#include <iostream>

namespace quazal
{
    enum PacketFlags: uint8_t
    {
        ACK = 1 << 0,
        RELIABLE = 1 << 1,
        NEED_ACK = 1 << 2,
        HAS_SIZE = 1 << 3
    };

    enum PacketType: uint8_t
    {
        SYN = 0, CONNECT = 1, DATA = 2, DISCONNECT = 3, PING = 4,
        USER = 5, ROUTE = 6, RAW = 7
    };

    union TypeFlagsByte
    {
        uint8_t raw;
        
        struct {
            uint8_t flags: 5;
            uint8_t type: 3; 
        } bits;
    };

    namespace utils
    {
        PacketType ReadPacketType(uint8_t packetTypeFlagsByte);
        bool CheckFlag(uint8_t packetTypeFlagsByte, PacketFlags flag);
        uint8_t CreateTypeFlagsByte(PacketType type, uint8_t flags);
    }
}
