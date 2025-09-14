#pragma once

#include "vport.hpp"
#include "typeflags.hpp"
#include "ioutils.hpp"
#include "checksum.hpp"

#include <cstdint>
#include <optional>
#include <vector>

namespace quazal
{
    // Main PRUDP packet
    class qpacket
    {
    public:
        VPort source;
        VPort destination;
        uint8_t packetTypeFlags;
        uint8_t sessionId;
        uint32_t signature;
        uint16_t sequenceId;
        std::optional<uint32_t> connSignature;
        std::optional<uint16_t> fragmentId;
        std::optional<uint16_t> payloadSize;
        std::optional<std::vector<uint8_t>> payload; // Will be RMC 
        uint8_t checksum;

        qpacket()
            : packetTypeFlags(0),
            sessionId(0),
            signature(0),
            sequenceId(0),
            checksum(0)
        {}

        // Parse PRUDP packet
        qpacket(std::vector<uint8_t> data, std::string service_name)
        {
            ioutils rdr(data);

            source = VPort::from_byte(rdr.read<uint8_t>());
            destination = VPort::from_byte(rdr.read<uint8_t>());

            std::cout << "source type: " << +source.type << std::endl;
            std::cout << "source port: " << +source.port << std::endl;
            std::cout << "destination type: " << +destination.type << std::endl;
            std::cout << "destination port: " << +destination.port << std::endl;

            packetTypeFlags = rdr.read<uint8_t>();

            PacketType type = utils::ReadPacketType(packetTypeFlags);
            std::cout << "Packet type: " << static_cast<int>(type) << std::endl;

            for (int i = 0; i < 5; ++i)
            {
                std::cout << "  Flag " << i << ": "
                    << ((packetTypeFlags & (1 << (i + 3))) ? "1" : "0") << std::endl;
            }

            sessionId = rdr.read<uint8_t>();
            std::cout << "sessionId: " << +sessionId << std::endl;

            signature = rdr.read<uint32_t>();
            std::cout << "signature: " << +signature << std::endl;

            sequenceId = rdr.read<uint16_t>();
            std::cout << "sequenceId: " << +sequenceId << std::endl;

            if (type == PacketType::SYN || type == PacketType::CONNECT)
            {
                connSignature = rdr.read<uint32_t>();
                std::cout << "connSignature: " << +(*connSignature) << std::endl;

                if (service_name == "grfs_secure" && type == PacketType::CONNECT)
                {
                    auto remaining = rdr.remaining();
                    std::vector<uint8_t> pl = rdr.read_bytes(remaining - 1);

                    payload = pl;

                    std::cout << "payload: [ ";
                    for (auto b : pl)
                        std::cout << std::hex << +b << " ";
                    std::cout << "]" << std::dec << std::endl;
                }
            }

            if (type == PacketType::DATA)
            {
                fragmentId = rdr.read<uint8_t>();
                std::cout << "fragmentId: " << +(*fragmentId) << std::endl;

                if (utils::CheckFlag(packetTypeFlags, PacketFlags::HAS_SIZE))
                {
                    payloadSize = rdr.read<uint16_t>();
                    std::cout << "payloadSize: " << +(*payloadSize) << std::endl;

                    std::vector<uint8_t> pl = rdr.read_bytes(*payloadSize);
                    payload = pl;

                    std::cout << "payload: [ ";
                    for (auto b : pl)
                        std::cout << std::hex << +b << " ";
                    std::cout << "]" << std::dec << std::endl;
                }
                else
                {
                    auto remaining = rdr.remaining();
                    std::vector<uint8_t> pl = rdr.read_bytes(remaining - 1);

                    payload = pl;

                    std::cout << "payload: [ ";
                    for (auto b : pl)
                        std::cout << std::hex << +b << " ";
                    std::cout << "]" << std::dec << std::endl;
                }
            }

            checksum = rdr.read<uint8_t>();
            std::cout << "checksum: " << +checksum << std::endl;
        }

        std::vector<uint8_t> serialize(bool autoChecksum = true)
        {
            ioutils wtr;

            wtr.write<uint8_t>(source.to_byte());
            wtr.write<uint8_t>(destination.to_byte());

            wtr.write<uint8_t>(packetTypeFlags);
            wtr.write<uint8_t>(sessionId);

            wtr.write<uint32_t>(signature);
            wtr.write<uint16_t>(sequenceId);

            PacketType type = utils::ReadPacketType(packetTypeFlags);

            if (type == PacketType::SYN || type == PacketType::CONNECT)
            {
                if (connSignature)
                    wtr.write<uint32_t>(*connSignature);

                if (utils::CheckFlag(packetTypeFlags, PacketFlags::HAS_SIZE)) {
                    if (payloadSize)
                        wtr.write<uint16_t>(*payloadSize);
                }

                if (payload)
                    wtr.write_vector(*payload);
            }

            if (type == PacketType::DATA)
            {
                if (fragmentId)
                    wtr.write<uint8_t>(*fragmentId);

                if (utils::CheckFlag(packetTypeFlags, PacketFlags::HAS_SIZE)) {
                    if (payloadSize)
                        wtr.write<uint16_t>(*payloadSize);
                }

                if (payload)
                {
                    if (utils::CheckFlag(packetTypeFlags, PacketFlags::HAS_SIZE))
                        wtr.write<uint16_t>(static_cast<uint16_t>(payload->size()));

                    wtr.write_vector(*payload);
                }
            }

            std::vector<uint8_t> dataWithoutChecksum = wtr.get_data();
                
            if (autoChecksum)
            {
                // Automatically calculate checksum for all packets
                uint8_t cs = make_checksum(dataWithoutChecksum.data(), dataWithoutChecksum.size(), 0xFF);
                wtr.write<uint8_t>(cs);
                checksum = cs;
            }
            else
            {
                wtr.write<uint8_t>(checksum);
            }

            return wtr.get_data();
        }
    };
}
