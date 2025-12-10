#pragma once

#include <iostream>
#include <format>
#include <string>

namespace onlineconfig {
    std::string get_onlineconfig_response(std::string& addr, int& auth_port);
}