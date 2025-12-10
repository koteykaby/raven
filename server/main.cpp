#include "spdlog/spdlog.h"

#include "config/configuration.h"
#include "config/logrunner.h"

#include "onlineconfig/http.h"
#include "quazal/win_server.h"

#include <memory>
#include <string>

int main() {
    LoadConfig();
    LoadLogger();

    SPDLOG_INFO("raven quazal rendez-vouz dedicated server :> | made by koteykaby");
    SPDLOG_DEBUG("[Address: {}, OnlineConfigPort: {}, AuthPort: {}, SecurePort: {}, LogLevel: {}]", server_config.address, server_config.onlineconfigport, server_config.authport, server_config.secureport, server_config.loglevel);

    // Running services would be here
    std::thread httpThread([&]() {
        try {
            onlineconfig::serve(server_config.address, 
                server_config.onlineconfigport, 
                server_config.authport);
        } catch (const std::exception& e) {
            SPDLOG_ERROR("[HTTP SERVER] ERROR: {}", e.what());
        }
    });

    auto authServer = std::make_shared<Quazal::Server>(
        "auth",
        server_config.address,
        server_config.authport
    );

    auto secureServer = std::make_shared<Quazal::Server>(
        "secure",
        server_config.address,
        server_config.secureport
    );
    
    httpThread.join();

    return 0;
}