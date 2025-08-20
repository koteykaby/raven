#include "hooks.hpp"

#include "mpatcher.hpp"
#include "logger.hpp"

#include "addresses.hpp"
#include "config.hpp"

namespace hooks
{
    namespace patch
    {
        using namespace hooks::addresses::DX9;
        using namespace raven_loader::config;

        static constexpr auto ONLINE_CONFIG_SERVICE_SIZE = sizeof("onlineconfigservice.ubi.com");

        void ApplyPatches()
        {
            if (Config.PatchOnlineConfigServiceAddress == true)
            {
                utils::mpatcher::Patch(ONLINE_CONFIG_SERVICE_ADDR, Config.OnlineConfigServiceAddress.c_str(), ONLINE_CONFIG_SERVICE_SIZE);
            }
        }
    }
}