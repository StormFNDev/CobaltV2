#include <iostream>
#include <fstream>
//#include <shlobj_core.h>
#include <Windows.h>
#include <sstream>
#include "codes.h"
#include "settings.h"
#include "util.h"

namespace Colors
{
    int cyan = 3;
    int lightRed = 12;
    int green = 10;
    int defaultGray = 7;
}

const char* CharPtrToConst(char* c) { return c; }
std::string FloatToStr(float f) { return std::to_string(f); }
std::string IntToStr(int i) { return std::to_string(i); }
//const char* StrToConst(std::string s) { return s.c_str(); }
std::string ConstToStr(const char* c) { return c; }
std::string PathToStr(fs::path p)
{
    std::string pstr = p.u8string();
    return pstr;
}
std::string BoolToStr(bool b)
{
    if (b) return "true";
    else if (!b) return "false";
    return IntToStr(BOOL_TO_STR_FAILED);
}


void Log(int msg, int color, bool bNl, bool err)
{
    if (err)
    {
        Log(ConstToStr("Error Code: #") + IntToStr(msg), Colors::lightRed, true, false, false);
        WriteToLog(ConstToStr("Error Code: #") + IntToStr(msg));
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    std::cout << msg;
    if (bNl) nl();
}

void Log(std::string msg, int color, bool bNl, bool debug, bool rainbowLog)
{
    if (debug)
    {
        if (bIsProd) return;
    }
    if (rainbowLog)
    {
        std::string msgStr = msg;
        bool t = RainbowLog(msgStr);
        if (t)
        {
            if (bNl) nl();
        }
        return;
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    std::cout << msg;
    if (bNl) nl();
}

void nl() { std::cout << std::endl; }

//template <typename T>
bool RainbowLog(std::string msg) // TODO: Add support for const char etc.
{
    /*if (!checkType(msg, "string"))
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, Colors::lightRed);
        std::cout << "[ERROR] ";
        SetConsoleTextAttribute(hConsole, Colors::defaultGray);
        std::cout << "Type was not string!";
        return false;
    } */
    int msgIndex = 0;
    for (int k = 1; k < msg.length() + 1; k++)
    {
        if (msgIndex == msg.length() + 1)
        {
            nl();
            return true;
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, k);
        std::cout << msg[msgIndex];
        msgIndex++;
        if (k == 15) k = 1;
    }
    return true;
}

bool fexists(const fs::path& p)
{
    if (exists(p)) return true;
    else return false;
}

bool fdelete(const fs::path& p)
{
    if (!fexists(p)) return false;
    remove(p);
    WriteToLog(ConstToStr("Deleted ") + PathToStr(p));
}

void fcreate(const fs::path& p)
{
    if (fexists(p)) return;
    fs::create_directory(p);
    WriteToLog(ConstToStr("Created ") + PathToStr(p));
}

const char* GetEnv(const char* envname)
{
    // SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
    char* pValue;
    size_t len;
    errno_t err = _dupenv_s(&pValue, &len, envname);
    return CharPtrToConst(pValue);
}

std::string CobaltPath = GetEnv("APPDATA") + ConstToStr("\\United Backend\\CobaltV2");

std::string GetCobaltPath() { return CobaltPath; }

void InitAD()
{
    if (fexists(CobaltPath)) return;
    fcreate(CobaltPath);
    WriteToLog("Running InitAD..");
    return;
}

template <typename T>
void WriteToLog(T msg)
{
    std::ofstream outfile;
    outfile.open(CobaltPath + "\\logs.txt", std::ios::app); // TODO(Milxnor): name the files the time and date so the logs dont reset every time idk.
    if (!outfile.is_open()) Log(FSTREAM_NOT_OPEN, Colors::defaultGray, true, true);
    outfile << msg << std::endl;
    outfile.close();
}

