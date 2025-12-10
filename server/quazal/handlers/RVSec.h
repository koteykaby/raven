#pragma once

#include "../base/packet.h"

#include <WinSock2.h>

#include "../managers/ClientManager.h"

namespace Quazal {
    namespace RVSecHandler {
        std::vector<uint8_t> handlePacket(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager);

        std::vector<uint8_t> SYN(QPacket& packet, sockaddr_in& clientEndpoint, ClientManager& clientManager);
    }
}