#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include "spdlog/spdlog.h"

#include "response.h"

#pragma comment(lib, "ws2_32.lib")

namespace onlineconfig
{
    void serve(std::string& address, int& port, int& auth_port)
    {
        auto Response = onlineconfig::get_onlineconfig_response(address, auth_port);
        
        std::string result =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: " + std::to_string(Response.size()) + "\r\n\r\n" +
                    Response;

        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            SPDLOG_ERROR("WSAStartup failed");
            return;
        }

        SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == INVALID_SOCKET)
        {
            SPDLOG_ERROR("Socket creation failed");
            WSACleanup();
            return;
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(address.c_str());

        if (bind(server, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            SPDLOG_ERROR("Bind failed on {}:{}", address, port);
            closesocket(server);
            WSACleanup();
            return;
        }

        if (listen(server, SOMAXCONN) == SOCKET_ERROR)
        {
            SPDLOG_ERROR("Listen failed on {}:{}", address, port);
            closesocket(server);
            WSACleanup();
            return;
        }

        SPDLOG_INFO("HTTP Server started on {}:{}", address, port);

        while (true)
        {
            sockaddr_in clientAddr;
            int clientSize = sizeof(clientAddr);

            SOCKET client = accept(server, (sockaddr*)&clientAddr, &clientSize);
            if (client == INVALID_SOCKET)
                continue;

            char clientIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
            int clientPort = ntohs(clientAddr.sin_port);

            SPDLOG_INFO("Client connected from {}:{}", clientIp, clientPort);

            char buffer[1024] = {0};
            int bytesReceived = recv(client, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0)
            {
                std::string request(buffer);
                auto pos = request.find("\r\n");
                std::string firstLine = (pos != std::string::npos) ? request.substr(0, pos) : request;
                
                SPDLOG_INFO("[Request] {}:{} -> {}", clientIp, clientPort, firstLine);

                send(client, result.c_str(), result.size(), 0);
            }

            closesocket(client);
        }

        closesocket(server);
        WSACleanup();
    }

}