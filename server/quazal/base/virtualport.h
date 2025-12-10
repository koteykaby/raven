#pragma once

#include <cstdint>
#include <string>

namespace Quazal {
    enum StreamType
    {
        DO = 1,
        RV = 2,
        RVSec = 3,
        SBMGMT = 4,
        NAT = 5,
        SessionDiscovery = 6,
        NATEcho = 7,
        Routing = 8,
    };

    struct VirtualPort
    {
        StreamType type;
        uint8_t port;

        static VirtualPort from_byte(uint8_t b);
        uint8_t to_byte() const;

        std::string get_stream_type_from_byte() const;
        std::string to_string() const;

        bool operator<(const VirtualPort& other) const {
            return port < other.port;
        }
    };
}
    