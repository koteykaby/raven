#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <queue>
#include <mutex>

#include "managers/ClientManager.h"
#include "managers/VPortManager.h"

#include "spdlog/spdlog.h"

namespace Quazal {
    class Server {
    public:
        struct QueuedPacket {
            std::vector<uint8_t> data;
            sockaddr_in client;
        };
        Server(std::string serviceName, std::string ipAddress, uint16_t port);
        ~Server();

        void listen(SOCKET serverSocket);
        void worker(SOCKET serverSocket);

        void serve(std::string serviceName, std::string ipAddress, uint16_t port);
        void stop();
    private:
        SOCKET serverSocket;
        bool isRunning;

        std::string serviceName;
        std::string ipAddress;
        uint16_t port;

        std::mutex mtx;
        std::condition_variable cv;
        std::deque<QueuedPacket> packetQueue;
        
        ClientManager clientManager;
        VPortManager vportManager;
    };
}