#include "virtualport.h"

using namespace Quazal;

VirtualPort VirtualPort::from_byte(uint8_t b) {
    uint8_t t = b >> 4;
    uint8_t p = b & 0x0F;

    return { static_cast<StreamType>(t), p };
}

uint8_t VirtualPort::to_byte() const {
    return (static_cast<uint8_t>(type) << 4) | (port & 0x0F);
}

std::string VirtualPort::get_stream_type_from_byte() const {
    switch (type) {
        case DO:               return "DO";
        case RV:               return "RV";
        case RVSec:            return "RVSec";
        case SBMGMT:           return "SBMGMT";
        case NAT:              return "NAT";
        case SessionDiscovery: return "SessionDiscovery";
        case NATEcho:          return "NATEcho";
        case Routing:          return "Routing";
        default:               return "Unknown";
    }
}

std::string VirtualPort::to_string() const {
    return "VirtualPort { Type: " + get_stream_type_from_byte() +
           ", Port: " + std::to_string(port) + " }";
}
