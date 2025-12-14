#include "RVSec.h"

#include "../base/typeflags.h"
#include "../base/compression.h"
#include "../base/rc4.h"

#include "stream.h"

#include "../managers/ClientManager.h"
#include "../managers/RMCManager.h"

#include "spdlog/spdlog.h"
#include "utils/fmtutility.h"

namespace Quazal::RVSecHandler {

std::vector<uint8_t> handlePacket(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager, SOCKET& serverSocket, RMCDispatcher* rmcDispatcher) {
    SPDLOG_DEBUG("Handling RVSec stream packet");

    std::vector<uint8_t> rmcResponse;

    switch (packet.type) {
        case PacketType::SYN:
            return SYN(packet, clientEndpoint, clientManager);
        case PacketType::CONNECT:
            SPDLOG_DEBUG("Handling CONNECT packet");
            return CONNECT(packet, clientEndpoint, clientManager);
        case PacketType::DATA:
            SPDLOG_DEBUG("Handling DATA packet");
            rmcResponse = DATA_RMC(packet, clientEndpoint, clientManager, serverSocket, rmcDispatcher);

            return DATA_ACK(packet, clientEndpoint, clientManager);
        case PacketType::DISCONNECT:
            SPDLOG_DEBUG("Handling DISCONNECT packet");
            return DISCONNECT(packet, clientEndpoint, clientManager);
        case PacketType::PING:
            SPDLOG_DEBUG("Handling PING packet");
            return PING(packet, clientEndpoint, clientManager);
        default:
            SPDLOG_WARN("Unknown RVSec packet type {}!", static_cast<int>(packet.type));
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

    response.sessionId = packet.sessionId;

    response.signature = packet.signature;
    /*
        in the SYN packet, the connSignature is used to identify the connection
        and after this it will be signature
        it should be generated uniquely for each new connection. 
    */
    response.connSignature = clientManager.createClient(clientEndpoint);

    auto client = clientManager.getClient(response.connSignature.value());
    client->clientSignature = response.connSignature.value();
    client->clientSequenceId = packet.sequenceId;

    response.sequenceId = packet.sequenceId;

    SPDLOG_DEBUG("Responding to SYN with connSignature 0x{:X}", response.connSignature.value());

    auto bytes = response.serialize();

    return bytes;
}

std::vector<uint8_t> CONNECT(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager) {
    QPacket response;

    // Client connection signature is client
    auto client = clientManager.getClient(packet.signature);

    // Client gives server a connnection signature in CONNECT packet
    client->serverSignature = packet.connSignature.value();

    response.source = packet.destination;
    response.destination = packet.source;

    response.type = PacketType::CONNECT;
    response.flags = PacketFlags::ACK | PacketFlags::HAS_SIZE;

    response.sessionId = packet.sessionId;
	response.signature = packet.connSignature.value();
	response.sequenceId = packet.sequenceId;
	response.connSignature = 0x00000000;

    response.payload = { 0x0f };

	return response.serialize();
}

std::vector<uint8_t> DATA_ACK(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager) {
    QPacket response;
    auto client = clientManager.getClient(packet.signature);
        if (!client) {
        SPDLOG_ERROR(
            "DATA packet for unknown client, connSignature=0x{:X}",
            packet.connSignature.value()
        );
        return {};
    }

    response.source = packet.destination;
    response.destination = packet.source;
    
    response.type = PacketType::DATA;
    response.flags = PacketFlags::ACK;

    response.sessionId = client->serverSessionId;
    response.signature = client->serverSignature;
    
    response.sequenceId = packet.sequenceId;

    return response.serialize();
}

std::vector<uint8_t> DATA_RMC(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager, SOCKET& serverSocket, RMCDispatcher* rmcDispatcher) {
    QPacket response;
    RMC rmcRequest;
	std::vector<uint8_t> decompressed_payload;

    if (packet.payload) {
        std::vector<uint8_t> decrypted_payload = Quazal::RC4_Decrypt(*packet.payload, RMC_RC4);

        if (decompress_zlib(decrypted_payload, decompressed_payload) == true)
		{
			// and parse..
			rmcRequest = RMC::from_bytes(decompressed_payload);
		} else {
            rmcRequest = RMC::from_bytes(decrypted_payload);
        }
    }

    std::vector<uint8_t> rmcResult = rmcDispatcher->ProcessRequest(rmcRequest);

    return {};
}

std::vector<uint8_t> DISCONNECT(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager) {
    return {};
}

std::vector<uint8_t> PING(QPacket& packet, sockaddr_in& client, ClientManager& clientManager) {
    return {};
}

} // namespace Quazal::RVSecHandler
