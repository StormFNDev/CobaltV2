#include <Windows.h>
#include "main.h"
#include "kiero.h"
#include "MinHook.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Main();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        MH_Uninitialize();
        kiero::shutdown();
        break;
    }
    return TRUE;
}

/*

CobaltV1 Features:
NON-Skidded
Ingame
Error handling (too much lmfao)
String encryption
SSL Bypass
Private/Hybrid Mode
Readfile

CobaltV2 Features:
- Everything cobalt v1 had
- WAY better logging system
- Rainbow Log ( :Gay_hand: )
- ProcessEvent Hook
- VEH/MH Hooking Options
- Cleaner Code.
- No kick soon
- S13 And Prod Modes

*/