#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include "../macros.hpp"

#pragma comment(lib, "ws2_32.lib")

void startTCP(int port, const std::string service_name) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed\n";
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    // TODO parse from config
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(port); 

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    LOG_INFO("{} running at {}", service_name, port);

    while (true) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Accept failed\n";
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        LOG_INFO("service:{} -> new connection from {}:{}", service_name, clientIP, clientAddr.sin_port);

        // Creating the onlineconfig
        // TODO parse IP-address from config file
        std::string payload = R"([{"Name":"SandboxUrl","Values":["prudp:\/address=127.0.0.1;port=21030"]},{"Name":"SandboxUrlWS","Values":["127.0.0.1:21030"]}])";

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Cache-Control: private\r\n"
            "Content-Length: " + std::to_string(payload.length()) + "\r\n"
            "\r\n" + payload;

        // and sending it to the game
        send(clientSocket, response.c_str(), strlen(response.c_str()), 0);
        LOG_INFO("{} <- {} payload: {}", service_name, clientIP, payload);

        // do not do that. game is crying because of that. seriously.
        //closesocket(clientSocket);
        //LOG_INFO("service: {} client_ip: {} disconnected!", service_name, clientIP);
    }

    closesocket(serverSocket);
    WSACleanup();
}