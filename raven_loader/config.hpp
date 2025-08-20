#pragma once

#include "ini.hpp"

#include <string>

namespace raven_loader
{
    namespace config
    {
        struct RavenConfig
        {
            // [Account]
            std::string Username;
            std::wstring Password;  

            // [DedicatedServer]
            std::string OnlineConfigServiceAddress;

            // [Network]
            std::string ClientIP;

            // [Hooks]
            bool PatchOnlineConfigServiceAddress;
            bool LogHermesEvents;
            bool LogRMCEvents;
        };

        void Initialize();

        extern RavenConfig Config;
    }
}