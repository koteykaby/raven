#pragma once

#include <cstdint>
#include <vector>
#include <functional>
#include <mutex>
#include <map>

#include "../base/virtualport.h"
#include "../base/packet.h"

#include "ClientManager.h"

namespace Quazal {
    using VPortHandler = std::function<std::vector<uint8_t>(QPacket&, sockaddr_in&, ClientManager&)>;

    class VPortManager {
    private:
        std::map<VirtualPort, VPortHandler> portHandlers;
        std::mutex mutex;
    public:
        void registerHandler(const VirtualPort& vport, VPortHandler handler);
        std::vector<uint8_t> handlePacket(
            const VirtualPort& vp,
            QPacket& packet,
            sockaddr_in& client,
            ClientManager& mgr
        );
    };
}