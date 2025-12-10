#pragma once

#include <string>

struct ServicesConfig
{
    // [Server]
    std::string address;
    // ports
    int onlineconfigport;
    int authport;
    int secureport;

    // [Settings]
    std::string loglevel;
};

void LoadConfig();

extern ServicesConfig server_config;