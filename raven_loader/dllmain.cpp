#include <Windows.h>

#include "logger.hpp"

#include "macro.hpp"
#include "hooks/hooks.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        raven_loader::config::Initialize();
        
        logger.info("ubiorbitapi_r2_loader.dll | Raven loader is ready!");

        hooks::patch::ApplyPatches();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}