#include "rc4.h"

#include <vector>
#include <string>
#include <cstdint>

namespace Quazal {
    // swap_bytes: small helper to swap two bytes in the state array.
    // Keeping this as a separate function makes the code clearer when
    // the KSA/PRGA perform many swaps on the S array.
    static void swap_bytes(uint8_t& a, uint8_t& b) {
        uint8_t tmp = a;
        a = b;
        b = tmp;
    }

    // rc4_init: Key-Scheduling Algorithm (KSA)
    // - Initializes the state array 's' with values 0..255.
    // - Mixes the state using the provided key so the initial permutation
    //   depends on the key. This prepares the S-box for the PRGA.
    static void rc4_init(uint8_t s[256], const std::string& key) {
        for (int i = 0; i < 256; ++i)
            s[i] = static_cast<uint8_t>(i);

        int j = 0;
        // For each position i, j is updated with the current state and key,
        // then s[i] and s[j] are swapped. This produces a key-dependent
        // initial permutation of the S array.
        for (int i = 0; i < 256; ++i) {
            j = (j + s[i] + static_cast<uint8_t>(key[i % key.size()])) & 0xFF;
            swap_bytes(s[i], s[j]);
        }
    }

    // RC4_Encrypt: Pseudo-Random Generation Algorithm (PRGA) + XOR with data
    // - Uses the initialized S array to generate a keystream byte for each input byte.
    // - The PRGA advances indices i and j, swaps S[i] and S[j], and selects a
    //   keystream byte k = S[(S[i] + S[j]) & 0xFF].
    // - The output byte is input_byte XOR keystream_byte.
    // Note: RC4 encryption and decryption are symmetric, so the same routine
    // can be used for both operations.
    std::vector<uint8_t> RC4_Encrypt(const std::vector<uint8_t>& data, const std::string& key) {
        uint8_t s[256];
        rc4_init(s, key);

        std::vector<uint8_t> output(data.size());
        int i = 0, j = 0;

        for (size_t n = 0; n < data.size(); ++n) {
            // advance i, update j using current state, swap, then produce keystream byte
            i = (i + 1) & 0xFF;
            j = (j + s[i]) & 0xFF;
            swap_bytes(s[i], s[j]);
            uint8_t k = s[(s[i] + s[j]) & 0xFF];
            output[n] = data[n] ^ k;
        }

        return output;
    }

    // RC4_Decrypt: identical to encryption because RC4 is symmetric.
    // Keep this wrapper for clarity.
    std::vector<uint8_t> RC4_Decrypt(const std::vector<uint8_t>& data, const std::string& key) {
        return RC4_Encrypt(data, key);
    }

} // namespace quazal
