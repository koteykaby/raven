#pragma once

#include <vector>
#include <cstdint>
#include <iostream>

const std::string RC4_KEY = "CD&ML"; 

inline void swap(uint8_t &a, uint8_t &b) {
    uint8_t tmp = a;
    a = b;
    b = tmp;
}

void rc4_init(uint8_t s[256]) {
    for (int i = 0; i < 256; ++i) s[i] = i;
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + s[i] + static_cast<uint8_t>(RC4_KEY[i % RC4_KEY.size()])) & 0xFF;
        swap(s[i], s[j]);
    }
}

std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& data) {
    uint8_t s[256];
    rc4_init(s);

    std::vector<uint8_t> output(data.size());
    int i = 0, j = 0;

    for (size_t n = 0; n < data.size(); ++n) {
        i = (i + 1) & 0xFF;
        j = (j + s[i]) & 0xFF;
        swap(s[i], s[j]);
        uint8_t k = s[(s[i] + s[j]) & 0xFF];
        output[n] = data[n] ^ k;
    }
    return output;
}

std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& data) {
    return Encrypt(data);
}