#pragma once

#include <string>

struct UplayConfig
{
    std::string username;
    std::string password;
    std::string gamekey;

    std::string server_ip;
    std::string port;
};

void LoadConfig();

extern UplayConfig config;