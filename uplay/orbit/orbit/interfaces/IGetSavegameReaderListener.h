#pragma once

#include "savegamereader.h"

namespace mg::orbitclient
{
    class IGetSavegameReaderListener
    {
    public:
    typedef void(__thiscall* CallBackPtrType)(void*, 
        unsigned int requestId, 
        int unk,
		SavegameReader* saveGameReader);

	void (**CallBackPtr)(unsigned int requestId, 
        int unk, 
        SavegameReader* saveGameReader);
    };
}