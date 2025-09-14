#pragma once

#include <cstdint>
#include <random>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

namespace quazal
{
    struct ClientInfo
    {
        sockaddr_in address;
        int size;
        std::string clientIP;
        uint16_t port;

        // default constructor. wont be used, required only for successful compile.
        ClientInfo() : address{}, size(0), clientIP(""), port(0) {}

        ClientInfo(sockaddr_in addr) : address(addr) {
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addr.sin_addr, ip_str, INET_ADDRSTRLEN);
            size = sizeof(addr);
            clientIP = std::string(ip_str);
            port = ntohs(addr.sin_port);
        }

        std::string addr_toString() const {
            return clientIP + ":" + std::to_string(port);
        }
    };

    // hashmap for better client indexing
    // the uint32 in this case is the connection id (signature)
    extern std::map<uint32_t, ClientInfo> clients;

    uint32_t RegisterClient(sockaddr_in addr);
    void RemoveClient(uint32_t connection_id);
}
