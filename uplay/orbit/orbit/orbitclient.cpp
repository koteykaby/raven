#include "orbitclient.h"

#include "spdlog/spdlog.h"

#include "config/configuration.h"

#include <memory>

mg::orbitclient::OrbitClient::OrbitClient()
{
    SPDLOG_INFO("OrbitClient constructed!");
}

mg::orbitclient::OrbitClient::~OrbitClient() 
{
    SPDLOG_INFO("OrbitClient destructed!~");
}

bool mg::orbitclient::OrbitClient::StartLauncher(unsigned int unk0, unsigned int unk1, char const* unk2, char const* unk3) {
    SPDLOG_INFO("StartLauncher called with args [ productId: {}, unk1: {}, langCode: {}, unk3: {} ]", unk0, unk1, unk2 ? unk2 : "null", unk3 ? unk3 : "null");
    bool result = false;
    SPDLOG_INFO("StartLauncher returning: {}", result);
    return result;
}

unsigned short* mg::orbitclient::OrbitClient::GetInstallationErrorString(char const* errorString) {
    SPDLOG_INFO("GetInstallationErrorString called with errorString: {}", errorString ? errorString : "null");
    auto result = nullptr;
    SPDLOG_INFO("GetInstallationErrorString returning nullptr");
    return result;
}

unsigned int mg::orbitclient::OrbitClient::GetInstallationErrorNum() {
    SPDLOG_INFO("GetInstallationErrorNum called");
    unsigned int result = 0;
    SPDLOG_INFO("GetInstallationErrorNum returning: {}", result);
    return result;
}

void mg::orbitclient::OrbitClient::Update() {} 

void mg::orbitclient::OrbitClient::GetLoginDetails(unsigned int requestId, IGetLoginDetailsListener* loginDetailsListenerCallBack) {
    SPDLOG_INFO("GetLoginDetails called with requestId: {}, listener: {}", requestId, 
        loginDetailsListenerCallBack ? "valid" : "null");

    const auto callBack = reinterpret_cast<IGetLoginDetailsListener::CallBackPtrType>(**loginDetailsListenerCallBack->CallBackPtr);

    SPDLOG_INFO("GetLoginDetails calling callback with username {} password {} gamekey {}", config.username, config.password, config.gamekey);

    callBack(loginDetailsListenerCallBack, requestId,
        config.username.c_str(),
        std::wstring(config.password.begin(), config.password.end()).c_str(),
        config.gamekey.c_str()
    );
}

unsigned int mg::orbitclient::OrbitClient::GetRequestUniqueId() {
    SPDLOG_INFO("GetRequestUniqueId called");
    unsigned int result = ++requestId;
    SPDLOG_INFO("GetRequestUniqueId returning: {}", result);
    return result;
}

void mg::orbitclient::OrbitClient::GetSavegameWriter(unsigned int requestId, IGetSavegameWriterListener* savegameWriterListenerCallBack,
    unsigned int productId, unsigned int saveGameId, bool open) 
{
    SPDLOG_INFO("GetSavegameWriter called with args [ requestId: {}, listener: {}, productId: {}, saveGameId: {}, open: {} ]",
        requestId, savegameWriterListenerCallBack ? "valid" : "null", productId, saveGameId, open);

    const auto callBack = reinterpret_cast<IGetSavegameWriterListener::CallBackPtrType>(**savegameWriterListenerCallBack->CallBackPtr);

    auto swriter = std::make_shared<SavegameWriter>();

    callBack(savegameWriterListenerCallBack, requestId, 0, swriter.get());
}

void mg::orbitclient::OrbitClient::GetSavegameReader(unsigned int requestId, IGetSavegameReaderListener* savegameReaderListenerCallBack,
    unsigned int productId, unsigned int saveGameId) 
{
    SPDLOG_INFO("GetSavegameReader called with args [ requestId: {}, listener: {}, productId: {}, saveGameId: {} ]",
        requestId, savegameReaderListenerCallBack ? "valid" : "null", productId, saveGameId);

    const auto callBack = reinterpret_cast<IGetSavegameReaderListener::CallBackPtrType>(**savegameReaderListenerCallBack->CallBackPtr);

    auto sreader = std::make_shared<SavegameReader>();

    callBack(savegameReaderListenerCallBack, requestId, 0, sreader.get());
}
