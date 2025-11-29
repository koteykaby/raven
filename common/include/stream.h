#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

class Stream
{
private:
    std::vector<uint8_t> buffer;
    size_t pos;

    void error(const char* msg) const
    {
        std::cout << "[Stream] ERROR: " << msg << "\n";
    }

public:
    Stream(const std::vector<uint8_t>& data);
    Stream();

    const std::vector<uint8_t>& get_data() const;
    size_t get_pos() const;
    void set_pos(size_t new_pos);

    template<typename T>
    T read()
    {
        if (pos + sizeof(T) > buffer.size()) {
            error("not enough data to read()");
            return T{};
        }

        T value;
        std::memcpy(&value, buffer.data() + pos, sizeof(T));
        pos += sizeof(T);
        return value;
    }

    std::vector<uint8_t> read_bytes(size_t size);
    void skip(size_t bytes);
    size_t remaining() const;

    template<typename T>
    void write(const T& value)
    {
        if (buffer.capacity() < buffer.size() + sizeof(T))
            buffer.reserve(buffer.size() + sizeof(T) + 1024);

        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
        buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
    }

    void write_bytes(const uint8_t* data, size_t size);
    void write_vector(const std::vector<uint8_t>& data);

    void clear();
};
