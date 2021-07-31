#pragma once

#include "xorstr.hpp"
#include <string>

constexpr float ver = 0.5;
constexpr bool bIsProd = true;
constexpr bool bIsS13 = false;
constexpr bool bWriteLogsToFile = false;
//#define READFILE // can't if statement bcuz ye
#define SENDLOGSTOCONSOLE
std::string FNhost = ENC("storm-fn.dev");
std::string ProxyHost = ENC("");

constexpr bool INGAME = true;
constexpr bool PROCESSEVENTHOOK = true;

#ifdef READFILE // man that 1kb is important ok :/
std::string myText;
std::string hybridyesorno;
std::ifstream MyReadFile = "info.txt";
#endif

#ifndef READFILE
constexpr bool bIsHybrid = true;
#endif

#ifdef READFILE
constexpr bool bIsHybrid;
#endif
