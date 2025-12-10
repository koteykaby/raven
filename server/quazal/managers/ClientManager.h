#pragma once

#include <unordered_map>
#include <cstdint>
#include <WinSock2.h>

namespace Quazal {
    struct ClientInfo {
        sockaddr_in endpoint;

        uint16_t serverSequenceId = 0;
        uint32_t serverSignature = 0xfb261505;
        uint8_t serverSessionId = 0xab;
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