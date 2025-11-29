#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "config/configuration.h"

#include "orbit/export.h"
#include "hooks/hooks.h"

#include <memory>

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("grfs-tracing.log", true);
        auto logger = std::make_shared<spdlog::logger>("file_logger", file_sink);

        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::info);

        spdlog::set_pattern("%H:%M:%S <%^%l%$:%!> %v");

        SPDLOG_INFO("DLL Attached!");

        LoadConfig();
        hooks::initialize();

        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
