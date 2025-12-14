#pragma once

#include <vector>
#include <cstdint>
#include <string>

inline const std::string RMC_RC4 = "CD&ML";

namespace Quazal {
    void rc4_init(uint8_t s[256], const std::string& key);
    std::vector<uint8_t> RC4_Encrypt(const std::vector<uint8_t>& data, const std::string& key);
    std::vector<uint8_t> RC4_Decrypt(const std::vector<uint8_t>& data, const std::string& key);
}
