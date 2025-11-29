#pragma once

#include "export.h"
#include "interfaces/ISavegameWriteListener.h"

#include <string>

namespace mg::orbitclient {
    class orbit_export SavegameWriter {
        std::string FilePath;
        std::string NamePath;
        unsigned int SaveId;

    public:
        SavegameWriter();
        void Close(bool arg);

        void Write(unsigned int requestId, 
            ISavegameWriteListener* savegameWriteListenerCallBack, 
            void* buff,
            unsigned int numberOfBytes);

        bool SetName(unsigned short* name);
    };
}