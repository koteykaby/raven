#pragma once

#include "export.h"
#include "interfaces/ISavegameReadListener.h"

#include <string>

namespace mg::orbitclient
{
    class orbit_export SavegameReader
    {
    private:
        std::string file_path;
    public:
        SavegameReader();
        void Close();

        void Read(unsigned int requestId, 
            ISavegameReadListener* 
            savegameReadListenerCallBack,
            unsigned int offset, 
            void* buff, 
            unsigned int numberOfBytes);
    };
}