#include "checksum.h"

namespace Quazal 
{
    // operations with the checksum was gained from the GROBackend
    // im not really understand what actually happened in it
    // well chatgpt created me not too good alternative.
    uint8_t get_protocol_setting(uint8_t protocol) {
        switch (protocol) {
        case 3: return 0xE3;
        case 1:
        case 5:
        default: return 0x00;
        }
    }
    uint8_t make_checksum(const uint8_t* data, size_t length, uint8_t setting) {
        if (!data || length == 0) return 0;

        if (setting == 0xFF) {
            uint8_t protocol = data[0] >> 4;
            setting = get_protocol_setting(protocol);
        }

        uint32_t sum32 = 0;
        size_t full_chunks = length / 4;

        for (size_t i = 0; i < full_chunks; i++) {
            uint32_t chunk;
            memcpy(&chunk, data + i * 4, 4);
            sum32 += chunk;
        }

        uint8_t remainder_sum = setting;
        size_t remainder = length % 4;

        for (size_t i = length - remainder; i < length; i++) {
            remainder_sum += data[i];
        }

        uint8_t result = 0;
        result += (sum32 >> 24) & 0xFF;
        result += (sum32 >> 16) & 0xFF;
        result += (sum32 >> 8) & 0xFF;
        result += sum32 & 0xFF;
        result += remainder_sum;

        return result;
    }
    // are you understood what actually happened?
    // huhhuuhuh...........
}