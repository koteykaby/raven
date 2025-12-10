#include "spdlog/spdlog.h"
#include "savegamewriter.h"

#include <string>
#include <codecvt>
#include <locale>

using namespace mg::orbitclient;

SavegameWriter::SavegameWriter()
{
    SPDLOG_INFO("SavegameWriter constructed");
}

void SavegameWriter::Close(bool arg)
{
    SPDLOG_INFO("Close called [ arg: {} ]", arg);
}

void SavegameWriter::Write(unsigned int requestId,
                           ISavegameWriteListener* savegameWriteListenerCallBack,
                           void* buff,
                           unsigned int numberOfBytes)
{
    // TODO
    SPDLOG_INFO("Write called [ requestId: {} listener: {} buff: {} bytes: {} ]",
        requestId,
        static_cast<void*>(savegameWriteListenerCallBack),
        buff,
        numberOfBytes);
}

bool SavegameWriter::SetName(unsigned short* name)
{
    if (!name) {
        SPDLOG_WARN("SetName called with null pointer");
        return true;
    }

    // конвертация UTF-16 → UTF-8
    std::u16string u16_name(reinterpret_cast<char16_t*>(name));
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::string utf8_name = converter.to_bytes(u16_name);

    SPDLOG_INFO("SetName called with name: {}", utf8_name);
    return true;
}
