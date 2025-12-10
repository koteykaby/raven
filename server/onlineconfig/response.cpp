#include "response.h"

#include "spdlog/spdlog.h"

std::string onlineconfig::get_onlineconfig_response(std::string& addr, int& auth_port) {
    std::string json = std::format(
        "[{{\"Name\":\"SandboxUrl\",\"Values\":[\"prudp:/address={};port={}\"]}},"
        "{{\"Name\":\"SandboxUrlWS\",\"Values\":[\"{}:{}\"]}}]",
        addr, auth_port, addr, auth_port
    );

    //SPDLOG_DEBUG("result: {}", json);

    return json;
}