// Compression implementation for RMC messages
#include "miniz.h"

#include <vector>
#include <iostream>

bool decompress_zlib(const std::vector<uint8_t>& compressedData, std::vector<uint8_t>& outData);
std::vector<uint8_t> compress_zlib(const std::vector<uint8_t>& input);