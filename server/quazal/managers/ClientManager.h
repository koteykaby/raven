#pragma once

#include <unordered_map>
#include <cstdint>
#include <WinSock2.h>

namespace Quazal {
    struct ClientInfo {
        sockaddr_in endpoint;

        uint16_t serverSequenceId;
        uint32_t serverSignature;
        uint8_t serverSessionId;

        uint16_t clientSequenceId;
        uint32_t clientSignature;
        uint8_t clientSessionId;
    };

    class ClientManager {
    private:
        /*
            uint32 here is the client connection signature from prudp
        */
        std::unordered_map<uint32_t, ClientInfo> clientsMap;
    public:
        /*
            register new client
            returns connection signature of new client
        */
        uint32_t createClient(const sockaddr_in& endpoint);

        // get ptr to client
        ClientInfo* getClient(uint32_t connSignature);
    };
}