#pragma once

#include <cstdint>
#include <vector>
#include <functional>
#include <mutex>
#include <map>
#include <any>
#include <WinSock2.h>

#include "../base/virtualport.h"
#include "../base/packet.h"
#include "ClientManager.h"

namespace Quazal {

    // Универсальный обработчик пакета с контекстом
    using VPortHandler = std::function<std::vector<uint8_t>(
        QPacket&, sockaddr_in&, ClientManager&, SOCKET&, std::any ctx
    )>;

    class VPortManager {
    private:
        std::map<VirtualPort, VPortHandler> portHandlers;
        std::mutex mutex;
    public:
        void registerHandler(const VirtualPort& vport, VPortHandler handler);

        // Теперь handlePacket принимает контекст
        std::vector<uint8_t> handlePacket(
            const VirtualPort& vp,
            QPacket& packet,
            sockaddr_in& client,
            ClientManager& mgr,
            SOCKET& serverSocket,
            std::any ctx = {}
        );
    };
}
