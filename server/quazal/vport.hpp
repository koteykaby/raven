#pragma once

#include <cstdint>
#include <iostream>

namespace quazal
{
    enum StreamType
    {
        DO = 1,
        RV = 2,
        OldRVSec = 3,
        SBMGMT = 4,
        NAT = 5,
        SessionDiscovery = 6,
        NATEcho = 7,
        Routing = 8,
        Game = 9,
        RVSecure = 10,
        Relay = 11
    };

    struct VPort
    {
        uint8_t type;
        uint8_t port;

        static VPort from_byte(uint8_t raw)
        {
            uint8_t type = raw >> 4;
            uint8_t port = raw & 0x0F;
            return { type, port };
        }

        uint8_t to_byte() const
        {
            return (type << 4) | (port & 0x0F);
        }
    };

}