#pragma once
#include "settings.h"
#include <Windows.h>
#include "util.h"
#include "curl.h"
#include "hook.h"

void Main()
{
    if (!bHasReset)
    {
        FS::fdelete(FS::CobaltPath + "\\logs.txt");
        bHasReset = !bHasReset;
        FS::fcreate(FS::CobaltPath + "\\logs.txt");
    }
    std::string b = ", ";
    FS::WriteToLog(ConstToStr("Settings: ") + FloatToStr(ver) + b + BoolToStr(bIsProd) + b + BoolToStr(bIsS13) + b + BoolToStr(bWriteLogsToFile) + b + BoolToStr(bHasReset) + b + BoolToStr(INGAME) + b + BoolToStr(PROCESSEVENTHOOK) + b + BoolToStr(bIsHybrid));
    FS::WriteToLog("Redirecting streams..");
    FILE* a;
    freopen_s(&a, ENC("CONIN$"), "w", stdin);
#ifdef SENDLOGSTOCONSOLE
    AllocConsole();
    freopen_s(&a, ENC("CONOUT$"), "w", stdout);
    freopen_s(&a, ENC("CONOUT$"), "w", stderr);
#endif
#ifdef WRITELOGSTOFILE
#ifndef SENDLOGSTOCONSOLE // It doesn't send to console and logs atm.
    freopen_s(&a, ENC("logs.txt"), "w", stdout); // Writes processevent (logs) to file.
#endif
#endif
    Hooking::Hook((void*)CurlEasyOptP, Curl::CurlHook, (void**)&_curl_easy_setopt, HookMethod);
    const char* ascii = ENC(R"(
_________     ______        _____________    _______
__  ____ / ________ / _______ ___ / _ / __ |  / /_ | __ \
_ / _  __ \_  __ \  __ `/_ / _  __ / _ | / /____/ /
/ /___  / /_/ /  /_/ / /_/ /_  / / /_ __ |/ / _  __ /
\____ / \____//_.___/\__,_/ /_/  \__/ _____/  /____/ 
)");
    Logs::Log(ascii);
    Logs::Log("\nLaunching CobaltV2!"); //(" + ver);
    Logs::DebugLog("Redirecting to " + FNhost);
}
