#include "ClientManager.h"

#include <random>
#include <cstdint>

#include "spdlog/spdlog.h"

namespace Quazal {
    uint32_t ClientManager::createClient(const sockaddr_in& endpoint)
    {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

        uint32_t connSignature;

        do {
            connSignature = dist(rng);
        } while (clientsMap.find(connSignature) != clientsMap.end());

        ClientInfo info{};
        info.endpoint = endpoint;
        info.serverSequenceId = 0;
        info.serverSignature = dist(rng);
        info.serverSessionId = 0;

        clientsMap.emplace(connSignature, info);

        SPDLOG_INFO("Created new client with connSignature 0x{:X}", connSignature);

        return connSignature;
    }

    ClientInfo* ClientManager::getClient(uint32_t connSignature)
    {
        auto it = clientsMap.find(connSignature);
        if (it != clientsMap.end()) {
            return &it->second;
        }
        
        SPDLOG_ERROR("Client with connSignature 0x{:X} not found", connSignature);
        
        return nullptr;
    }
}