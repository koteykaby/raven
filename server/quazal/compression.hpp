#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include "deps/miniz.h"

std::vector<uint8_t> compress_data(const uint8_t* data, size_t size, int level = MZ_BEST_COMPRESSION) {
    if (!data || size == 0) return {};

    mz_ulong compress_bound = compressBound(size);
    std::vector<uint8_t> compressed(compress_bound);

    int status = compress2(compressed.data(), &compress_bound, data, size, level);
    if (status != MZ_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed.resize(compress_bound); 
    return compressed;
}

std::vector<uint8_t> decompress_data(const uint8_t* data, size_t size, size_t original_size) {
    if (!data || size == 0) return {};

    std::vector<uint8_t> decompressed(original_size);
    mz_ulong decompressed_size = original_size;

    int status = uncompress(decompressed.data(), &decompressed_size, data, size);
    if (status != MZ_OK) {
        throw std::runtime_error("Decompression failed");
    }

    return decompressed;
}
