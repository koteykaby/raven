#pragma once

#include "virtualport.h"
#include "typeflags.h"
#include "checksum.h"

#include <cstdint>
#include <optional>
#include <vector>
#include <string>

namespace Quazal {
    class QPacket {
    public:
        // Source virtual port
        VirtualPort source;
        // Destination virtual port
        VirtualPort destination;
        // Type and flags of the packet
        uint8_t packetTypeFlags;
        // Divide it to type and flags
        PacketType type;
        // Create a vector with all enabled flags
        uint8_t flags;
        // Session ID
        uint8_t sessionId;
        // Packet signature
        uint32_t signature;
        // Sequence ID 
        uint16_t sequenceId;
        // Connection signature (Appears only in SYN and CONNECT packets)
        std::optional<uint32_t> connSignature;
        // Fragment ID (if packet is fragmented (appears only in DATA packets))
        std::optional<uint16_t> fragmentId;
        // Payload size (in big DATA packets)
        std::optional<uint16_t> payloadSize;
        // Packet payload (CONNECT and DATA packets)
        std::optional<std::vector<uint8_t>> payload;
        // Packet checksum 
        uint8_t checksum;

        QPacket() : 
            packetTypeFlags(0),
            sessionId(0),
            signature(0),
            sequenceId(0),
            checksum(0)
        {}
        
        // Parse PRUDP packet
        QPacket(std::vector<uint8_t> data, std::string serviceName);

        // Serialize PRUDP packet
        std::vector<uint8_t> serialize();

        void logPacket();
    };
}