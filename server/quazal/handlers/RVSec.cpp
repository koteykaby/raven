#include "RVSec.h"

#include "../base/typeflags.h"
#include "stream.h"

#include "../managers/ClientManager.h"

#include "spdlog/spdlog.h"
#include "utils/fmtutility.h"

namespace Quazal::RVSecHandler {

std::vector<uint8_t> handlePacket(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager) {
    SPDLOG_DEBUG("Handling RVSec stream packet");

    switch (packet.type) {
        case PacketType::SYN:
            return SYN(packet, clientEndpoint, clientManager);
        default:
            SPDLOG_WARN("Unknown RVSec packet type {}, response will be empty!", static_cast<int>(packet.type));
            return {};
    }
}

std::vector<uint8_t> SYN(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager) {
    QPacket response;

    SPDLOG_INFO("Client sent SYN packet, preparing SYN-ACK response");

    response.source = packet.destination;
    response.destination = packet.source;

    response.type = PacketType::SYN;
    response.flags = PacketFlags::ACK | PacketFlags::HAS_SIZE;

    response.packetTypeFlags = makeTypeFlags(response.type, response.flags);

    response.sessionId = packet.sessionId;

    response.signature = packet.signature;
    /*
        in the SYN packet, the connSignature is used to identify the connection
        and after this it will be signature
        it should be generated uniquely for each new connection. 
    */
    response.connSignature = clientManager.createClient(clientEndpoint);

    response.sequenceId = packet.sequenceId;
    response.payloadSize = 0x0000;

    SPDLOG_DEBUG("Responding to SYN with connSignature 0x{:X}", response.connSignature.value());

    auto bytes = response.serialize();

    SPDLOG_DEBUG("serialize returned {} bytes", bytes.size());

    if (!bytes.empty())
        SPDLOG_DEBUG("dump: {}", fmtToHex(bytes));

    return bytes;
}

} // namespace Quazal::RVSecHandler
