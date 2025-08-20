#include "SavegameWriter.hpp"

#include "logger.hpp"

void mg::orbitclient::SavegameWriter::Write(unsigned int requestId, ISavegameWriteListener* savegameWriteListenerCallBack, 
                   void* buff, unsigned int numberOfBytes)
{
    logger.debug("call!: mg::orbitclient::SavegameWriter::Write");
}
void mg::orbitclient::SavegameWriter::Close(bool arg)
{
    logger.debug("call!: mg::orbitclient::SavegameWriter::Close");
}
bool mg::orbitclient::SavegameWriter::SetName(unsigned short* name)
{
    logger.debug("call!: mg::orbitclient::SavegameWriter::SetName");
    return true;
}