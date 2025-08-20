#include "logger.hpp"

#include "SavegameReader.hpp"

void mg::orbitclient::SavegameReader::Close() {
    logger.debug("call!: mg::orbitclient::SavegameReader::Close");
}

void mg::orbitclient::SavegameReader::Read(unsigned int requestId, 
    ISavegameReadListener* savegameReadListenerCallBack,
    unsigned int offset, void* buff, unsigned int numberOfBytes) 
{
    logger.debug("call!: mg::orbitclient::SavegameReader::Read");
}