#pragma once

namespace mg::orbitclient {
	class ISavegameWriteListener {
	public:
		typedef void(__thiscall* CallBackPtrType)(void*, 
            unsigned int requestId, 
            unsigned int bytesWritten);
		void (**CallBackPtr)(unsigned int requestId, 
            unsigned int bytesWritten);
	};
}