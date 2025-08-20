#pragma once

#include "macro.hpp"
#include "Interfaces/ISavegameWriteListener.hpp"

#include <iostream>

namespace mg {
    namespace orbitclient {
        class ORBIT_EXPORT SavegameWriter {
            std::string FilePath;
            std::string NamePath;
            unsigned int SaveId;
        public:
            void Close(bool arg);
            void Write(unsigned int requestId, ISavegameWriteListener* savegameWriteListenerCallBack, void* buff,
                unsigned int numberOfBytes);
            bool SetName(unsigned short* name);
        };
    }
} 