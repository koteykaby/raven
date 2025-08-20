#include "ini.hpp"

#include "config.hpp"

namespace raven_loader
{
    namespace config
    {
        INIParser parser;
        RavenConfig Config;

        void Initialize()
        {
            if (!parser.Load("raven.ini")) 
            {
                MessageBoxW(NULL, L"Couldn't load raven.ini configurations!", L"ubiorbitapi_r2_loader.dll", 
                            MB_ICONWARNING | MB_OK);
                return;
            }

            Config.Username = parser.GetString("Account", "Username");
            Config.Password = parser.GetWString("Account", "Password");

            Config.OnlineConfigServiceAddress = parser.GetString("DedicatedServer", "OnlineConfigService");

            Config.ClientIP = parser.GetString("Network", "IP");

            Config.PatchOnlineConfigServiceAddress = parser.GetBool("Hooks", "PatchOnlineConfigServiceAddress");
            Config.LogHermesEvents = parser.GetBool("Hooks", "LogHermesEvents");
            Config.LogRMCEvents = parser.GetBool("Hooks", "LogRMCEvents");
        }
    }
}