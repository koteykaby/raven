#include "addresses.h"

#include "spdlog/spdlog.h"

const std::unordered_map<std::string, uintptr_t>* hooks::LoadBinaryAddresses()
{
    char fullPath[MAX_PATH];
    GetModuleFileNameA(NULL, fullPath, MAX_PATH);

    char filename[_MAX_FNAME];
    char ext[_MAX_EXT];
    
    _splitpath_s(fullPath, NULL, 0, NULL, 0, filename, _MAX_FNAME, ext, _MAX_EXT);
    
    std::string fname(filename);

    if (fname == "Future Soldier DX9") {
        SPDLOG_INFO("Binary is DX9, loading hooks::addresses_dx9");
        return &hooks::addresses_dx9;
    }
    if (fname == "Future Soldier DX11") {
        SPDLOG_INFO("Binary is DX11, loading hooks::addresses_dx11");
        return &hooks::addresses_dx11;
    }
    else {
        SPDLOG_ERROR("Unknown binary loaded! Binary name {}", fname);
    }
}

std::unordered_map<std::string, uintptr_t> hooks::addresses_dx9 = {
    {"HermesOutputLog", 0x00F9F720}
};

std::unordered_map<std::string, uintptr_t> hooks::addresses_dx11 = {
    {"HermesOutputLog", 0x008AA460}
};