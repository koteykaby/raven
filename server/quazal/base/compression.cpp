#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>

#include "compression.h" 

#include "spdlog/spdlog.h"

bool decompress_zlib(const std::vector<uint8_t>& data, std::vector<uint8_t>& outData) {
    std::vector<uint8_t> signature = {0x78, 0x9C};

    auto it = std::search(data.begin(), data.end(), signature.begin(), signature.end());

    if (it == data.end()) {
        SPDLOG_ERROR("Signature 78 9C not found!");
        return false;
    }

    size_t offset = std::distance(data.begin(), it);

    std::vector<uint8_t> compressedData(data.begin() + offset, data.end());

    mz_ulong decompressedSize = 10 * compressedData.size(); 
    outData.resize(decompressedSize);

    int status = mz_uncompress(outData.data(), &decompressedSize,
                               compressedData.data(), compressedData.size());
    if (status != MZ_OK) {
        SPDLOG_ERROR("Decompression failed, status: {}", status);
        return false;
    }

    outData.resize(decompressedSize); 
    return true;
}

std::vector<uint8_t> compress_zlib(const std::vector<uint8_t>& input) {
    size_t buf_size = input.size() + input.size() / 100 + 12; 
    std::vector<uint8_t> compressed(buf_size);

    mz_ulong compressed_size = buf_size;
    int result = mz_compress2(compressed.data(), &compressed_size,
                              input.data(), input.size(), MZ_DEFAULT_LEVEL);
    if (result != MZ_OK) {
        SPDLOG_ERROR("Compression failed, code {}", result);
        return {};
    }

    compressed.resize(compressed_size);
    return compressed;
}