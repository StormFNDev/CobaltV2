#include <string>
#include <iostream>
#include <fstream>
#include <shlobj_core.h>
#include <sstream>
#include <filesystem>
#include "codes.h"
#include "settings.h"

namespace Colors
{
    int cyan = 3;
    int lightRed = 12;
    int green = 10;
    int defaultGray = 7;
}

namespace FS
{
    template <typename T>
    static void WriteToLog(T msg);
}

namespace fs = std::filesystem;

static const char* CharPtrToConst(char* c) { return c; }
static std::string FloatToStr(float f) { return std::to_string(f); }
static std::string IntToStr(int i) { return std::to_string(i); }
//static const char* StrToConst(std::string s) { return s.c_str(); }
static std::string ConstToStr(const char* c) { return c; };
static std::string PathToStr(fs::path p)
{
    std::string pstr = p.u8string();
    return pstr;
}
static std::string BoolToStr(bool b)
{
    if (b) return "true";
    else if (!b) return "false";
    return IntToStr(ErrCodes::BOOL_TO_STR_FAILED);
}


namespace Logs
{

    static void nl() { std::cout << std::endl; }

    //template <typename T>
    static bool RainbowLog(std::string msg) // TODO: Add support for const char etc.
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

    static void Log(std::string msg, int color = Colors::defaultGray, bool bNl = true, bool rainbowLog = false)
    {
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

    static void Log(int msg, int color = Colors::defaultGray, bool bNl = true)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << msg;
        if (bNl) nl();
    }

    template <typename T>
    static void DebugLog(T msg, int color = Colors::defaultGray, bool bNl = true, bool rainbowLog = false)
    {
        if (!bIsProd) Log(msg, color, bNl, rainbowLog);
    }

    static void LogErr(int code) 
    { 
        Log(ConstToStr("Error Code: #") + IntToStr(code), Colors::lightRed, false); 
    }
}

namespace FS
{
    static bool fexists(const fs::path& p)
    {
        if (fs::exists(p)) return true;
        else return false;
    }

    static bool fdelete(const fs::path& p)
    {
        if (!fexists(p)) return false;
        fs::remove(p);
        WriteToLog(ConstToStr("Deleted ") + PathToStr(p));
    }

    static void fcreate(const fs::path& p)
    {
        if (fexists(p)) return;
        fs::create_directory(p);
        WriteToLog(ConstToStr("Created ") + PathToStr(p));
    }

    static const char* GetEnv(const char* envname)
    {
        // SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
        char* pValue;
        size_t len;
        errno_t err = _dupenv_s(&pValue, &len, envname);
        return CharPtrToConst(pValue);
    }

    std::string CobaltPath = GetEnv("APPDATA") + ConstToStr("\\United Backend\\CobaltV2");

    static void InitAD()
    {
        if (fexists(CobaltPath)) return;
        fcreate(CobaltPath);
        WriteToLog("Running InitAD..");
        return;
    }

    template <typename T>
    static void WriteToLog(T msg)
    {
        std::ofstream outfile;
        outfile.open(CobaltPath + "\\logs.txt", std::ios::app); // TODO(Milxnor): name the files the time and date so the logs dont reset every time idk.
        if (!outfile.is_open()) Logs::LogErr(ErrCodes::FSTREAM_NOT_OPEN);
        outfile << msg << std::endl;
        outfile.close();
    }

}
