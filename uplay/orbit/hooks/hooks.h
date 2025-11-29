#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "addresses.h"

namespace hooks 
{
    namespace detours 
    {
        // getaddrinfo hook to redirect game traffic
        typedef INT (__stdcall *GETADDRINFO)(
            PCSTR pNodeName,
            PCSTR pServiceName,
            const ADDRINFOA *pHints,
            PADDRINFOA *ppResult
        );
        
        // hook to change selected adapter by the game
        // TODO

        // Hermes functions
        typedef int HermesOutputLog(int logChannel, char *Format, ...);

        // windows debug function
        typedef void (WINAPI* OutputDebugStringADetour)(LPCSTR lpOutputString);
    }

    INT __stdcall hook_getaddrinfo(
        PCSTR pNodeName,
        PCSTR pServiceName,
        const ADDRINFOA *pHints,
        PADDRINFOA *ppResult
    );

    int hook_HermesOutputLog(int logChannel, char *Format, ...);
    void WINAPI hook_OutputDebugStringA(LPCSTR lpOutputString);

    // Hooks initializer
    bool initialize();
}