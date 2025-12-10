#include "fmtutility.h"

#include "spdlog/fmt/fmt.h"

std::string fmtToHex(std::vector<uint8_t> data)
{
    std::string out;

    for (size_t i = 0; i < data.size(); i++)
        out += fmt::format("0x{:02X} ", data[i]);

    return out;
}