#include "packet.h"

#include "typeflags.h"
#include "virtualport.h"

#include "stream.h"

#include "spdlog/spdlog.h"
#include "utils/fmtutility.h"

Quazal::QPacket::QPacket(std::vector<uint8_t> data, std::string serviceName)
{
    // Initialize binary reader
    Stream rdr(data);

    // Read VPorts
    source = VirtualPort::from_byte(rdr.read<uint8_t>());
    destination = VirtualPort::from_byte(rdr.read<uint8_t>());

    // Read packet type and flags
    packetTypeFlags = rdr.read<uint8_t>();

    type = extractType(packetTypeFlags);
    flags = extractFlags(packetTypeFlags);

    sessionId = rdr.read<uint8_t>();
    signature = rdr.read<uint32_t>();
    sequenceId = rdr.read<uint16_t>();

    // Read optional fields
    if (type == PacketType::SYN || type == PacketType::CONNECT)
    {
        connSignature = rdr.read<uint32_t>();
    }
    if (type == PacketType::DATA)
    {
        fragmentId = rdr.read<uint8_t>();

        if (flags & PacketFlags::HAS_SIZE)
        {
            payloadSize = rdr.read<uint16_t>();
        }

        payload = rdr.read_bytes(rdr.remaining() - 1); // Exclude checksum
    }

    checksum = rdr.read<uint8_t>();
}

void Quazal::QPacket::logPacket()
{
    SPDLOG_DEBUG(
        "QPacket [source={}, dest={}, packetTypeFlags=0x{:X}, "
        "sessionId=0x{:X}, signature=0x{:X}, sequenceId={}, "
        "type={}, flags=0x{:X}, connSignature=0x{:X}, "
        "fragmentId={}, payloadSize={}, payload={}, checksum=0x{:X}]",
        
        source.to_string(),
        destination.to_string(),
        packetTypeFlags,
        sessionId,
        signature,
        sequenceId,
        static_cast<uint8_t>(type),
        flags,

        connSignature.value_or(0),
        fragmentId.value_or(0),
        payloadSize.value_or(0),

        fmtToHex(payload.value_or(std::vector<uint8_t>{})),

        checksum
    );
}

std::vector<uint8_t> Quazal::QPacket::serialize()
{
    Stream wtr;

    // Write VPorts
    wtr.write(source.to_byte());
    wtr.write(destination.to_byte());

    // Write packet type and flags
    wtr.write(packetTypeFlags);

    wtr.write(sessionId);
    wtr.write(signature);
    wtr.write(sequenceId);

    // Write optional fields
    if (type == PacketType::SYN || type == PacketType::CONNECT)
    {
        wtr.write(connSignature.value());
    }
    if (type == PacketType::DATA)
    {
        wtr.write(fragmentId.value());
    }
    if (flags & PacketFlags::HAS_SIZE)
    {
        wtr.write(payloadSize.value());
    }
    if (type == PacketType::DATA)
    {
        wtr.write_vector(payload.value());
    }

    std::vector<uint8_t> pkt = wtr.get_data();

    // Calculate checksum
    uint8_t checksum = make_checksum(pkt.data(), pkt.size(), 0xFF);

    // Write checksum as last byte
    wtr.write<uint8_t>(checksum);

    logPacket();

    return wtr.get_data();
}
