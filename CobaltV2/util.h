#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace Colors
{
    extern int cyan;
    extern int lightRed;
    extern int green;
    extern int defaultGray;
}

const char* CharPtrToConst(char* c);
std::string FloatToStr(float f);
std::string IntToStr(int i);
//const char* StrToConst(std::string s);
std::string ConstToStr(const char* c);
std::string PathToStr(fs::path p);
std::string BoolToStr(bool b);
void Log(int msg, int color = Colors::defaultGray, bool bNl = true, bool err = false);
void Log(std::string msg, int color = Colors::defaultGray, bool bNl = true, bool debug = false, bool rainbowLog = false);
void nl();
bool RainbowLog(std::string msg);

bool fexists(const fs::path& p);
bool fdelete(const fs::path& p);
void fcreate(const fs::path& p);
const char* GetEnv(const char* envname);
std::string GetCobaltPath();
void InitAD();
template <typename T>
void WriteToLog(T msg);
