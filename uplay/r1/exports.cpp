#include "exports.h"

extern "C" r1_export int UPLAY_Init() {
    return 1;
}
extern "C" r1_export int UPLAY_FRIENDS_Init() {
    return 1;
}
extern "C" r1_export int UPLAY_Quit() {
    return 1;
}
extern "C" r1_export int UPLAY_GetNextEvent() {
    return 1;
}
extern "C" r1_export int UPLAY_Update() {
    return 0;
}
extern "C" r1_export int UPLAY_FRIENDS_GetFriendList() {
    return 1;
}