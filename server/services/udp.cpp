#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#include "../quazal/qpacket.hpp"
#include "../quazal/qhandler.hpp"
#include "../quazal/clientinfo.hpp"
#include "../macros.hpp"
#include "udp.hpp"

std::string bytes_ashex(const void* data, size_t length) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    std::stringstream hex_stream;

    for (size_t i = 0; i < length; i++) {
        hex_stream << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(bytes[i]) << " ";
    }

    return hex_stream.str();
}

void serveUDP(int port, const std::string service_name)
{
    using namespace quazal;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    LOG_INFO("{} running at {}", service_name, port);

    char buffer[1024];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    while (true) {
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
            (sockaddr*)&clientAddr, &clientAddrSize);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

        LOG_INFO("{} -> recvfrom {} data: [ {}]", service_name, clientIP, bytes_ashex(buffer, bytesReceived));

        std::vector<uint8_t> received_data(buffer, buffer + bytesReceived);
        qpacket pkt(received_data, service_name);

        std::vector<uint8_t> result = quazal::handle(pkt, clientAddr);

        if (result.empty())
        {
            LOG_WARNING("{} <- {} Nothing to send, packet is empty", service_name, clientIP);
        }
        else
        {
            sendto(serverSocket,
                reinterpret_cast<const char*>(result.data()),
                result.size(),
                0,
                (sockaddr*)&clientAddr,
                clientAddrSize);

            LOG_INFO("{} <- sendto {} data: [ {}]", service_name, clientIP, bytes_ashex(result.data(), result.size()));
        }
    }

    closesocket(serverSocket);
    WSACleanup();
}