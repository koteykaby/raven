#pragma once

#include "MinHook.h"
#include "spdlog/spdlog.h"

#define HOOK_BY_ADDR(addr_expr, hook_fn, orig_ptr, friendly_name) \
do { \
    void* _target_addr = reinterpret_cast<void*>(addr_expr); \
    if (!_target_addr) { \
        SPDLOG_ERROR("{} address is null!", (friendly_name)); \
        return false; \
    } \
    if (MH_CreateHook(_target_addr, reinterpret_cast<LPVOID>(hook_fn), reinterpret_cast<LPVOID*>(&(orig_ptr))) != MH_OK) { \
        SPDLOG_ERROR("Failed to create {} hook!", (friendly_name)); \
        return false; \
    } \
    if (MH_EnableHook(_target_addr) != MH_OK) { \
        SPDLOG_ERROR("Failed to enable {} hook!", (friendly_name)); \
        return false; \
    } \
    SPDLOG_INFO("{} hook installed at address {:#x}", (friendly_name), reinterpret_cast<uintptr_t>(_target_addr)); \
} while(0)

#define HOOK_BY_NAME(module_handle, proc_name_cstr, hook_fn, orig_ptr, friendly_name) \
do { \
    void* _addr = reinterpret_cast<void*>(GetProcAddress((module_handle), (proc_name_cstr))); \
    if (!_addr) { \
        SPDLOG_ERROR("Failed to find {} in module ({}).", (proc_name_cstr), (friendly_name)); \
        return false; \
    } \
    if (MH_CreateHook(_addr, reinterpret_cast<LPVOID>(hook_fn), reinterpret_cast<LPVOID*>(&(orig_ptr))) != MH_OK) { \
        SPDLOG_ERROR("Failed to create {} hook!", (friendly_name)); \
        return false; \
    } \
    if (MH_EnableHook(_addr) != MH_OK) { \
        SPDLOG_ERROR("Failed to enable {} hook!", (friendly_name)); \
        return false; \
    } \
    SPDLOG_INFO("{} hook installed at address {:#x}", (friendly_name), reinterpret_cast<uintptr_t>(_addr)); \
} while(0)