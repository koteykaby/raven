#include "spdlog/spdlog.h"

#include <fstream>

#include "savegamereader.h"

mg::orbitclient::SavegameReader::SavegameReader() 
{
    SPDLOG_INFO("Call");
}

void mg::orbitclient::SavegameReader::Close() 
{
    SPDLOG_INFO("Call");
}

void mg::orbitclient::SavegameReader::Read(unsigned int requestId, 
    ISavegameReadListener* savegameReadListenerCallBack,
    unsigned int offset, void* buff, unsigned int numberOfBytes) 
{
    SPDLOG_INFO("Call [requestId {} listener {} offset {} buff {} numberOfBytes {}]",
        requestId,
        static_cast<void*>(savegameReadListenerCallBack),
        offset,
        buff,
        numberOfBytes
    );

    /*
        !!! NOT IMPLEMENTED
        in this part of code something should be readen, but i have no idea what exactly
        maybe someone knows? uhuhuh....
        game always call this but never calls savegame writer
        and for some reason it even have its own saves implementation
        
        ubisoft devs are funny..
    */
    std::ifstream file("uplay/save/savegame.dat", std::ios::binary);
    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open savegame.dat");
        return;
    }

    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(buff), numberOfBytes);
    unsigned int bytesRead = static_cast<unsigned int>(file.gcount());
    file.close();

    SPDLOG_INFO("Read {} bytes from savegame.dat", bytesRead); 

    if (savegameReadListenerCallBack && savegameReadListenerCallBack->CallBackPtr) {
        auto callback = *reinterpret_cast<ISavegameReadListener::CallBackPtrType*>(
            savegameReadListenerCallBack->CallBackPtr);
        if (callback) {
            SPDLOG_INFO("Invoking callback with requestId {} bytesRead {}", requestId, bytesRead);
            callback(savegameReadListenerCallBack, requestId, bytesRead);
        } else {
            SPDLOG_WARN("Callback pointer is null");
        }
    } else {
        SPDLOG_WARN("savegameReadListenerCallBack or CallBackPtr is null");
    }
}
