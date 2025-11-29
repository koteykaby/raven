#include <Windows.h>

#define r1_export __declspec(dllexport)

extern "C" r1_export int UPLAY_Init();
extern "C" r1_export int UPLAY_FRIENDS_Init();
extern "C" r1_export int UPLAY_Quit();
extern "C" r1_export int UPLAY_GetNextEvent();
extern "C" r1_export int UPLAY_Update();
extern "C" r1_export int UPLAY_FRIENDS_GetFriendList(); 