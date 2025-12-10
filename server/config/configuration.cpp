#include "configuration.h"

#include "ini.h"

#include <string>

ServicesConfig server_config;

void LoadConfig()
{
    ini* parser = new ini();
    parser->parse("data/service.ini");

    server_config.address = parser->data["Server"]["Address"];
    server_config.onlineconfigport = std::stoi(parser->data["Server"]["OnlineConfigPort"]);
    server_config.authport = std::stoi(parser->data["Server"]["AuthPort"]);
    server_config.secureport = std::stoi(parser->data["Server"]["SecurePort"]);

    server_config.loglevel = parser->data["Settings"]["LogLevel"];

    delete parser;
}