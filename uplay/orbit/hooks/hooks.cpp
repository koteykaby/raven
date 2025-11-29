#include "hooks.h"
#include "util.h"

#include "MinHook.h"
#include "spdlog/spdlog.h"

#include "config/configuration.h"

using namespace hooks;

uintptr_t getAddr(const std::unordered_map<std::string, uintptr_t>* addrMap, const std::string& func) {
    if (!addrMap) return 0;

    auto it = addrMap->find(func);
    if (it != addrMap->end()) {
        return it->second;
    }

    SPDLOG_ERROR("Function {} not found in address map!", func);
    return 0;
}

// Initializing pointers to original functions
detours::GETADDRINFO original_getaddrinfo = NULL;
detours::OutputDebugStringADetour original_OutputDebugStringA = nullptr;

// binary functions
detours::HermesOutputLog* original_HermesOutputLog = nullptr;

// hook log
void WINAPI hooks::hook_OutputDebugStringA(LPCSTR lpOutputString)
{
    SPDLOG_INFO("{}", lpOutputString);

    original_OutputDebugStringA(lpOutputString);
}

// Redirect game traffic from original ubi services to new ones
INT __stdcall hooks::hook_getaddrinfo(
    PCSTR pNodeName,
    PCSTR pServiceName,
    const ADDRINFOA *pHints,
    PADDRINFOA *ppResult
) {
    if (pNodeName && strcmp(pNodeName, "onlineconfigservice.ubi.com") == 0) {
        if (!pServiceName || strcmp(pServiceName, "80") == 0) {
            SPDLOG_INFO("Redirecting onlineconfigservice.ubi.com -> {}:{}", config.server_ip, config.port);
            return original_getaddrinfo(config.server_ip.c_str(), config.port.c_str(), pHints, ppResult);
        }
    }
}

// Hermes::toString(int Channel)
std::string get_log_level(int logChannel) 
{
    std::string result;

    switch (logChannel)
    {
        case 0:
            result = "Error";
            break;
        case 1:
            result = "Warning";
            break;
        case 2:
            result = "Info";
            break;
        case 3:
            result = "Sample";
            break;
        case 4:
            result = "System";
            break;
        case 5:
            result = "Result";
            break;
        case 6:
            result = "Debug1";
            break;
        case 7:
            result = "Debug2";
            break;
        default:
            SPDLOG_INFO("Unknown LogChannel");
            result = "???";
            break; 
    }
    return result;
}

int hooks::hook_HermesOutputLog(int logChannel, char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    char buffer[4096];
    vsnprintf(buffer, sizeof(buffer), Format, args);

    va_end(args);

    SPDLOG_INFO("{}: {}", get_log_level(logChannel), buffer);

    return original_HermesOutputLog(logChannel, (char*)"%s", buffer);
}

bool hooks::initialize()
{
    SPDLOG_INFO("Trying to initialize hooks!");

    // Load hashmap with all addresses
    const std::unordered_map<std::string, uintptr_t>* addrMap = hooks::LoadBinaryAddresses();

    if (MH_Initialize() != MH_OK) {
        SPDLOG_ERROR("Failed to initialize MinHook!");
        return false;
    }

    HOOK_BY_NAME(
        GetModuleHandle(L"kernel32.dll"),
        "OutputDebugStringA",
        &hook_OutputDebugStringA,
        original_OutputDebugStringA,
        "OutputDebugStringA"
    );

    HOOK_BY_NAME(
        GetModuleHandle(L"ws2_32.dll"),
        "getaddrinfo",
        &hook_getaddrinfo,
        original_getaddrinfo,
        "getaddrinfo"
    );

    // Enable binary hooks
    HOOK_BY_ADDR(
        getAddr(addrMap, "HermesOutputLog"),
        &hook_HermesOutputLog,
        original_HermesOutputLog,
        "HermesOutputLog"
    );

    SPDLOG_INFO("Hooks initialized successfully!");
    return true;
}
