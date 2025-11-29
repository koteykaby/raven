#include "configuration.h"

#include "ini.h"

UplayConfig config;

void LoadConfig()
{
    ini* parser = new ini();
    parser->parse("uplay.ini");

    config.username = parser->data["user"]["username"];
    config.password = parser->data["user"]["password"];
    config.gamekey = parser->data["user"]["gamekey"];

    config.server_ip = parser->data["network"]["server_ip"];
    config.port = parser->data["network"]["port"];

    delete parser;
}