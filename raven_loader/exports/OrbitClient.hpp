#pragma once

#include "macro.hpp"

#include "SavegameReader.hpp"
#include "SavegameWriter.hpp"

#include "Interfaces/IGetLoginDetailsListener.hpp"
#include "Interfaces/IGetSavegameWriterListener.hpp"
#include "Interfaces/IGetSavegameReaderListener.hpp"
#include "Interfaces/ISavegameReadListener.hpp"
#include "Interfaces/ISavegameWriteListener.hpp"

#include <iostream>

namespace mg {
    namespace orbitclient {
        class ORBIT_EXPORT OrbitClient {
            volatile int RequestId;

        public:
            OrbitClient();

            void StartProcess(unsigned short*, unsigned short*, unsigned short*);
            bool StartLauncher(unsigned int, unsigned int, char const*, char const*);

            void GetSavegameWriter(unsigned int requestId, IGetSavegameWriterListener* savegameWriterListenerCallBack,
                unsigned int productId, unsigned int saveGameId, bool open);
            void GetSavegameReader(unsigned int requestId, IGetSavegameReaderListener* savegameReaderListenerCallBack,
                unsigned int productId, unsigned int saveGameId);

            void GetLoginDetails(unsigned int requestId, IGetLoginDetailsListener* loginDetailsListenerCallBack);

            unsigned int GetRequestUniqueId();

            unsigned short* GetInstallationErrorString(char const*);
            unsigned int GetInstallationErrorNum();

            void Update();

            ~OrbitClient();
        };
    }
}