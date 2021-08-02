#pragma once
#include "settings.h"
#include <string>
#include <iostream>
#include "memory.h"
#include "defs.h"
#include "xorstr.hpp"
#include <regex>
#include "util.h"

uintptr_t CurlEasyOptP;
uintptr_t CurlSetOptP;

typedef void CURL;

// original easyopt thing we will use to setuo curl and do normal execution
auto (*_curl_easy_setopt)(CURL* cURL, uintptr_t option, ...)->CURLcode;

class Curl
{
public:
    static auto CurlHook(CURL* Curl, uintptr_t opt, va_list info)->CURLcode
    {
        int OFF = 0;
        switch (opt)
        {
        case CURLOPT_NOPROXY:
            return _curl_easy_setopt(Curl, opt, ProxyHost);
            break;

        case CURLOPT_SSL_VERIFYPEER:
            return _curl_easy_setopt(Curl, opt, OFF);
            break;
        case CURLOPT_SSL_VERIFYHOST:
            return _curl_easy_setopt(Curl, opt, OFF);
            break;
        case CURLOPT_PINNEDPUBLICKEY:
            return CURLcode::CURLE_OK;
            break;
        case CURLOPT_URL:
        }
        return _curl_easy_setopt(Curl, opt, info);
    }
    Curl()
    {
        CurlEasyOptP = Memory::sigscan(ENC("89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 48 83 EC 28 48 85 C9"));
        CurlSetOptP = Memory::sigscan(ENC("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 33 ED 49 8B F0 48 8B D9"));
        if (INGAME)
        {
            LPVOID lpCurlSetopt;
            INT(*CurlSetoptCast)(LPVOID, INT, va_list) = NULL;
            CurlSetoptCast = reinterpret_cast<decltype(CurlSetoptCast)>(CurlSetOptP);
        }
    }
};
