#include <string>
#include <iostream>

namespace Colors
{
    int cyan = 3;
    int lightRed = 12;
    int green = 10;
    int defaultGray = 7;
}

class Logs
{
private:
    template <typename T>
    static bool RainbowLog(std::string msg) // TODO: Add support for const char etc.
    {
        if (!checkType(msg, "string"))
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, Colors::lightRed);
            std::cout << "[ERROR] ";
            SetConsoleTextAttribute(hConsole, Colors::defaultGray);
            std::cout << "Type was not string!";
            return false;
        }
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
public:

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
};