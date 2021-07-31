#pragma once
#include "settings.h"
#include "logger.h"
#include <Windows.h>

void Main()
{
    FILE* a;
    freopen_s(&a, ENC("CONIN$"), "w", stdin);
#ifdef SENDLOGSTOCONSOLE
    AllocConsole(); // BUG: Console does not get written to without logs enabled.
    freopen_s(&a, ENC("CONOUT$"), "w", stdout);
    freopen_s(&a, ENC("CONOUT$"), "w", stderr);
#endif
#ifdef WRITELOGSTOFILE
#ifndef SENDLOGSTOCONSOLE // It doesn't send to console and logs atm.
    freopen_s(&a, ENC("logs.txt"), "w", stdout); // Writes processevent (logs) to file.
#endif
#endif
    const char* ascii = ENC(R"(
_________     ______        _____________    _______
__  ____ / ________ / _______ ___ / _ / __ |  / /_ | __ \
_ / _  __ \_  __ \  __ `/_ / _  __ / _ | / /____/ /
/ /___  / /_/ /  /_/ / /_/ /_  / / /_ __ |/ / _  __ /
\____ / \____//_.___/\__,_/ /_/  \__/ _____/  /____/ 
)");
    Logs::Log(ascii);
    Logs::Log("\nLaunching CobaltV2!");
    Logs::DebugLog("Redirecting to " + FNhost);
}
