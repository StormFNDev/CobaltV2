#include <string>
#include <iostream>
#include <fstream>
#include <shlobj_core.h>
#include <sstream>
#include "util.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Conversions
{
    static const char* CharPtrToConst(char* msg) { return msg; }
    static std::string FloatToStr(float f)
    {
        std::ostringstream ss;
        ss << f;
        return ss.str();
    }
    static const char* StrToConst(std::string msg) { return msg.c_str(); }
    static std::string ConstToStr(const char* msg) { return msg; };
    static std::string PathToStr(fs::path msg)
    {
        std::string path_string = msg.u8string();
        return path_string;
    }
}

namespace Logs
{
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
                return;
            }
            Log(msg[msgIndex], k, false);
            msgIndex++;
            if (k == 15) k = 1;
        }
        return true;
    }
    static void nl() { std::cout << std::endl; }

    template <typename T>
    static void Log(T msg, int color = Colors::defaultGray, bool bNl = true, bool rainbowLog = false)
    {
        if (rainbowLog)
        {
            std::string msgStr = msg;
            bool t = RainbowLog(msgStr);
            if (t) (bNl)nl();
            return;
        }
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << msg;
        if (bNl) nl();
    }

    template <typename T>
    static void DebugLog(T msg, int color = Colors::defaultGray, bool bNl = true, bool rainbowLog = false)
    {
        if (!PROD) Log(msg, color, bNl, rainbowLog);
    }
}

namespace FS
{
    static bool fexists(const fs::path& p)
    {
        if (fs::exists(p)) return true;
        else return false;
    }

    static void fcreate(const fs::path& p)
    {
        if (fexists(p)) return;
        fs::create_directory(p);
        WriteToLog(Conversions::ConstToStr("Created ") + Conversions::PathToStr(p));
    }

    static const char* GetEnv(const char* envname)
    {
        // SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
        char* pValue;
        size_t len;
        errno_t err = _dupenv_s(&pValue, &len, envname);
        return Conversions::CharPtrToConst(pValue);
    }

    std::string CobaltPath = GetEnv("APPDATA") + Conversions::ConstToStr("\\United Backend\\CobaltV2");

    static void InitAD()
    {
        if (fexists(CobaltPath)) return;

    }

    template <typename T>
    static void WriteToLog(T msg)
    {
        const char* roamingAD = GetEnv("APPDATA");
        std::ofstream outfile;
        outfile.open(CobaltPath + "\\logs.txt"); // TODO(Milxnor): name the files the time and date so the logs dont reset every time idk.
        outfile << msg << std::endl;
        outfile.close();
    }

}
