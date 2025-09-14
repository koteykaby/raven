#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <cstring> 

class ioutils
{
private:
    std::vector<uint8_t> buffer;
    size_t pos;
    
public:
    ioutils(const std::vector<uint8_t>& data) : buffer(data), pos(0) {}
    
    ioutils() : pos(0) {}
    
    const std::vector<uint8_t>& get_data() const
    {
        return buffer;
    }
    
    size_t get_pos() const
    {
        return pos;
    }
    
    void set_pos(size_t new_pos)
    {
        if (new_pos > buffer.size())
        {
            throw std::out_of_range("position out of range");
        }
        pos = new_pos;
    }

    template<typename T>
    T read()
    {
        if (pos + sizeof(T) > buffer.size())
        {
            throw std::out_of_range("not enough data");
        }

        T value;
        std::memcpy(&value, buffer.data() + pos, sizeof(T));
        pos += sizeof(T);
        return value;
    }

    std::vector<uint8_t> read_bytes(size_t size)
    {
        if (pos + size > buffer.size())
            throw std::out_of_range("not enough data to read bytes");

        std::vector<uint8_t> result(buffer.begin() + pos, buffer.begin() + pos + size);
        pos += size;
        return result;
    }

    void skip(size_t bytes)
    {
        if (pos + bytes > buffer.size())
        {
            throw std::out_of_range("cannot skip beyond buffer");
        }
        pos += bytes;
    }

    size_t remaining() const
    {
        return buffer.size() - pos;
    }

    template<typename T>
    void write(const T& value)
    {
        if (buffer.capacity() < buffer.size() + sizeof(T))
        {
            buffer.reserve(buffer.size() + sizeof(T) + 1024); 
        }
        
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
        buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
    }
    
    void write_bytes(const uint8_t* data, size_t size)
    {
        buffer.insert(buffer.end(), data, data + size);
    }
    
    void write_vector(const std::vector<uint8_t>& data)
    {
        buffer.insert(buffer.end(), data.begin(), data.end());
    }
    
    void clear()
    {
        buffer.clear();
        pos = 0;
    }
};