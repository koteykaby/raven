#pragma once

#include "interfaces/IGetLoginDetailsListener.h"
#include "interfaces/IGetSavegameReaderListener.h"
#include "interfaces/IGetSavegameWriterListener.h"

#include "savegamereader.h"
#include "savegamewriter.h"

#include "export.h"

#include <memory>

namespace mg::orbitclient
{
    class orbit_export OrbitClient
    {
    private:
        volatile int requestId;
    public:
        OrbitClient();
        ~OrbitClient();

        bool StartLauncher(unsigned int unk0, unsigned int unk1, char const* unk2, char const* unk3);

        unsigned short* GetInstallationErrorString(char const* errorString);
        unsigned int GetInstallationErrorNum();

        void Update();

        void GetSavegameWriter(unsigned int requestId, IGetSavegameWriterListener* savegameWriterListenerCallBack,
            unsigned int productId, unsigned int saveGameId, bool open);
        void GetSavegameReader(unsigned int requestId, IGetSavegameReaderListener* savegameReaderListenerCallBack,
            unsigned int productId, unsigned int saveGameId);
        void GetLoginDetails(unsigned int requestId, IGetLoginDetailsListener* loginDetailsListenerCallBack);

        unsigned int GetRequestUniqueId();
    };
}