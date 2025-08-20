#include "logger.hpp"

#include "config.hpp"

#include "OrbitClient.hpp"

mg::orbitclient::OrbitClient::OrbitClient()
{
	logger.debug("call!: mg::orbitclient::OrbitClient::OrbitClient");
}

void mg::orbitclient::OrbitClient::StartProcess(unsigned short *, unsigned short *, unsigned short *)
{
	logger.debug("call!: mg::orbitclient::OrbitClient::StartProcess");
}

void mg::orbitclient::OrbitClient::GetSavegameReader(unsigned int requestId,
													IGetSavegameReaderListener *savegameReaderListenerCallBack,
													unsigned int productId, unsigned int saveGameId)
{
	logger.debug("call!: mg::orbitclient::OrbitClient::GetSavegameReader RequestId: %d GetSavegameReaderListenerCallBack: %x ProductId: %d SaveGameId: %d", requestId,
						reinterpret_cast<void *>(&savegameReaderListenerCallBack),
						productId, saveGameId);
}

bool mg::orbitclient::OrbitClient::StartLauncher(unsigned int a, unsigned int b, char const *langCode, char const *arguments)
{
    logger.debug("call!: mg::orbitclient::OrbitClient::StartLauncher");
    return false;
}

unsigned short *mg::orbitclient::OrbitClient::GetInstallationErrorString(char const *err)
{
	logger.debug("call!: mg::orbitclient::OrbitClient::GetInstallationErrorString");
	return nullptr;
}

unsigned int mg::orbitclient::OrbitClient::GetInstallationErrorNum()
{
	logger.debug("call!: mg::orbitclient::OrbitClient::GetInstallationErrorNum");
	return 0;
}

void mg::orbitclient::OrbitClient::GetSavegameWriter(unsigned int requestId,
													 IGetSavegameWriterListener *savegameWriterListenerCallBack,
													 unsigned int productId, unsigned int saveGameId, bool open)
{
	logger.debug("call!: mg::orbitclient::OrbitClient::GetSavegameWriter RequestId: %s GetSavegameWriterListenerCallBack: %x ProductId: %d SaveGameId: %d", requestId,
						reinterpret_cast<void *>(&savegameWriterListenerCallBack), productId, saveGameId);
}

void mg::orbitclient::OrbitClient::GetLoginDetails(unsigned int requestId,
													IGetLoginDetailsListener *loginDetailsListenerCallBack)
{
	logger.debug("call!: mg::orbitclient::OrbitClient::GetLoginDetails RequestId: %d LoginDetailsListenerCallBack: %x", requestId,
						reinterpret_cast<void *>(&loginDetailsListenerCallBack));

	const auto callBack = reinterpret_cast<IGetLoginDetailsListener::CallBackPtrType>(**loginDetailsListenerCallBack->CallBackPtr);

	if (callBack == nullptr)
	{
		return;
	}

    using namespace raven_loader::config;

    logger.debug("using account credentials: %s:%s", 
        Config.Username.c_str(), 
        Config.Password.c_str());

	callBack(loginDetailsListenerCallBack, requestId, 
        Config.Username.c_str(), 
        Config.Password.c_str(), 
        "AAAA-AAAA-AAAA-AAAA");
}

unsigned int mg::orbitclient::OrbitClient::GetRequestUniqueId()
{
	logger.debug("call!: mg::orbitclient::OrbitClient::GetRequestUniqueId");
	return RequestId++;
}

void mg::orbitclient::OrbitClient::Update()
{
	// do not log it!!!
}

mg::orbitclient::OrbitClient::~OrbitClient()
{
	logger.debug("call!: mg::orbitclient::OrbitClient::~OrbitClient");
}