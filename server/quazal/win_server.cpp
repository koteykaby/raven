#include "win_server.h"

#include "managers/ClientManager.h"
#include "managers/VPortManager.h"
#include "managers/RMCManager.h"

#include "handlers/RVSec.h"

#include "base/packet.h"
#include "base/virtualport.h"

#include "spdlog/spdlog.h"
#include "../utils/fmtutility.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

Quazal::Server::Server(std::string serviceName, std::string ipAddress, uint16_t port)
    : isRunning(false), serverSocket(INVALID_SOCKET)
{
    SPDLOG_INFO("Initializing server: {} on {}:{}", serviceName, ipAddress, port);

    WSAData wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET udpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSock == INVALID_SOCKET) {
        SPDLOG_ERROR("{}: Failed to create UDP socket: {}", serviceName, WSAGetLastError());
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

    if (bind(udpSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        SPDLOG_ERROR("{}: Failed to bind: {}", serviceName, WSAGetLastError());
        closesocket(udpSock);
        return;
    }

    this->serverSocket = udpSock;

    auto rmcDispatcher = std::make_shared<Quazal::RMCDispatcher>(serviceName);

    // Register vport handlers
    vportManager.registerHandler(
    { Quazal::StreamType::RVSec, 1 },
        [rmcDispatcher](QPacket& packet, sockaddr_in& client, ClientManager& mgr, SOCKET& sock, std::any ctx) -> std::vector<uint8_t> {
            return RVSecHandler::handlePacket(packet, client, mgr, sock, rmcDispatcher.get());
        }
    );

    serve(serviceName, ipAddress, port);
}

Quazal::Server::~Server() {
    stop();
}

void Quazal::Server::listen(SOCKET serverSocket) {
    SPDLOG_INFO("{}: listening at {}", serviceName, port);

    isRunning = true;

    uint8_t buffer[2048];

    while (isRunning) {
        sockaddr_in clientAddr{};
        int clientLen = sizeof(clientAddr);

        int bytes = recvfrom(
            serverSocket,
            (char*)buffer,
            sizeof(buffer),
            0,
            (sockaddr*)&clientAddr,
            &clientLen
        );

        std::vector<uint8_t> received(buffer, buffer + bytes);

        QueuedPacket packet{received, clientAddr};

        if (bytes <= 0) continue;

        SPDLOG_DEBUG("{}: Packet from {}:{}: {}", serviceName, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), fmtToHex(packet.data));

        {
            std::lock_guard lock(mtx);

            packetQueue.push_back(packet);
        }
        cv.notify_one();
    }
}

void Quazal::Server::worker(SOCKET serverSocket)
{
    while (isRunning) {

        QueuedPacket pkt;

        {
            std::unique_lock lock(mtx);
            cv.wait(lock, [&] { return !packetQueue.empty() || !isRunning; });

            if (!isRunning) break;

            pkt = std::move(packetQueue.front());
            packetQueue.pop_front();
        }

        Quazal::QPacket q(pkt.data, serviceName);
        std::vector<uint8_t> response;

        q.logPacket();

        response = vportManager.handlePacket(q.destination, q, pkt.client, clientManager, serverSocket);

        auto hex = fmtToHex(response);
        SPDLOG_DEBUG("{}: Response packet to {}:{}: {}",
            serviceName,
            inet_ntoa(pkt.client.sin_addr),
            ntohs(pkt.client.sin_port),
            hex
        );

        if (!response.empty()) {
            sendto(
                serverSocket,
                (char*)response.data(),
                response.size(),
                0,
                (sockaddr*)&pkt.client,
                sizeof(pkt.client)
            );
        }
    }
}

void Quazal::Server::serve(std::string serviceName, std::string ipAddress, uint16_t port) 
{
    this->serviceName = std::move(serviceName);
    this->ipAddress   = std::move(ipAddress);
    this->port        = port;

    isRunning = true;

    std::thread listenerThread([this] {
        listen(this->serverSocket);
    });

    std::thread workerThread([this] {
        worker(this->serverSocket);
    });

    listenerThread.detach();
    workerThread.detach();

    SPDLOG_INFO("{} started on {}:{}", this->serviceName, this->ipAddress, this->port);
}

void Quazal::Server::stop() 
{
    if (!isRunning) return;

    isRunning = false;
    cv.notify_all();

    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    SPDLOG_INFO("Server {} stopped", serviceName);
}