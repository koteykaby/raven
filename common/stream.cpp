#include "Stream.h"

Stream::Stream(const std::vector<uint8_t>& data)
    : buffer(data), pos(0)
{
}

Stream::Stream()
    : pos(0)
{
}

const std::vector<uint8_t>& Stream::get_data() const
{
    return buffer;
}

size_t Stream::get_pos() const
{
    return pos;
}

void Stream::set_pos(size_t new_pos)
{
    if (new_pos > buffer.size()) {
        error("set_pos() out of range");
        return;
    }

    pos = new_pos;
}

std::vector<uint8_t> Stream::read_bytes(size_t size)
{
    if (pos + size > buffer.size()) {
        error("not enough data to read_bytes()");
        return {};
    }

    std::vector<uint8_t> result(buffer.begin() + pos, buffer.begin() + pos + size);
    pos += size;
    return result;
}

void Stream::skip(size_t bytes)
{
    if (pos + bytes > buffer.size()) {
        error("skip() out of range");
        return;
    }

    pos += bytes;
}

size_t Stream::remaining() const
{
    return buffer.size() - pos;
}

void Stream::write_bytes(const uint8_t* data, size_t size)
{
    buffer.insert(buffer.end(), data, data + size);
}

void Stream::write_vector(const std::vector<uint8_t>& data)
{
    buffer.insert(buffer.end(), data.begin(), data.end());
}

void Stream::clear()
{
    buffer.clear();
    pos = 0;
}
