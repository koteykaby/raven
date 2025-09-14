#include "clientinfo.hpp"

#include <cstdint>
#include <random>
#include <unordered_map>
#include <winsock2.h>

#include "../macros.hpp"

namespace quazal
{
    std::map<uint32_t, ClientInfo> clients;

    uint32_t RegisterClient(sockaddr_in addr)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis;

        uint32_t connection_id = dis(gen);

        ClientInfo client(addr);

        clients[connection_id] = client;

        LOG_INFO("registered new client with conn signature {}", connection_id);

        return connection_id;
    }
    void RemoveClient(uint32_t connection_id)
    {
        clients.erase(connection_id);
    }
}
