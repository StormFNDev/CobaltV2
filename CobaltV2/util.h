namespace Conversions
{
	static const char* CharPtrToConst(char* msg);
	static std::string FloatToStr(float f);
	static const char* StrToConst(std::string msg);
	static std::string ConstToStr(const char* msg);
	static std::string PathToStr(fs::path msg);
}

namespace Colors
{
	extern int cyan = 3;
	extern int lightRed = 12;
	extern int green = 10;
	extern int defaultGray = 7;
}

namespace Logs
{
	static bool RainbowLog(std::string msg);
	static void nl();
	template <typename T>
	static void Log(T msg, int color = Colors::defaultGray, bool bNl = true, bool rainbowLog = false);
	template <typename T>
	static void DebugLog(T msg, int color = Colors::defaultGray, bool bNl = true, bool rainbowLog = false);
}

namespace FS
{
	static bool fexists(const fs::path& p);
	static void fcreate(const fs::path& p);
	static const char* GetEnv(const char* envname);
	static void InitAD();
	template <typename T>
	static void WriteToLog(T msg);
}