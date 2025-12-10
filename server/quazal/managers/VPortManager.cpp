#include "VPortManager.h"

#include "spdlog/spdlog.h"

void Quazal::VPortManager::registerHandler(const VirtualPort& vport, VPortHandler handler) {
    std::lock_guard<std::mutex> lock(mutex);
    portHandlers[vport] = handler;
}

std::vector<uint8_t> Quazal::VPortManager::handlePacket(
    const VirtualPort& vp,
    QPacket& packet,
    sockaddr_in& client,
    ClientManager& mgr
)
{
    std::lock_guard lk(mutex);

    auto it = portHandlers.find(vp);
    if (it == portHandlers.end()) {
        SPDLOG_ERROR("No handler for port {}:{}", static_cast<int>(vp.type), vp.port);
        return {};
    }

    SPDLOG_DEBUG("Found handler for port {}:{}", static_cast<int>(vp.type), vp.port);

    return it->second(packet, client, mgr);
}