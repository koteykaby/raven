#pragma once

#include "../base/packet.h"

#include <WinSock2.h>
#include <any>

#include "../managers/ClientManager.h"
#include "../managers/RMCManager.h"

namespace Quazal {
    namespace RVSecHandler {
        std::vector<uint8_t> handlePacket(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager, SOCKET& serverSocket, RMCDispatcher* rmcDispatcher);

        std::vector<uint8_t> SYN(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager);
        std::vector<uint8_t> CONNECT(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager);
        std::vector<uint8_t> DATA_ACK(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager);
        std::vector<uint8_t> DATA_RMC(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager, SOCKET& serverSocket, RMCDispatcher* rmcDispatcher);
        std::vector<uint8_t> DISCONNECT(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager);
        std::vector<uint8_t> PING(QPacket& packet, sockaddr_in& client, ClientManager& clientManager);
    }
}