#pragma once

#include "macro.hpp"

#include "Interfaces/ISavegameReadListener.hpp"

#include <iostream>

namespace mg {
    namespace orbitclient {
        class ORBIT_EXPORT SavegameReader
        {
            std::string FilePath;
        public:
            void Close();
            void Read(unsigned int requestId, ISavegameReadListener* savegameReadListenerCallBack,
                unsigned int offset, void* buff, unsigned int numberOfBytes);
        };
    }
} 