#pragma once
#include <memory>
#include <map>
#include <string>
#include <SDL.h>

class Config
{
public:
	static int getIntOption(const std::string& section, const std::string& option);
	static double getRealOption(const std::string& section, const std::string& option);
	static Uint32 getColorOption(const std::string& section, const std::string& option);
	static std::string getStringOption(const std::string& section, const std::string& option);
	static const SDL_Rect& getRect(const std::string& name);
private:
	struct ConfigValue
	{
		enum ValueType { CVT_UNKNOWN = 0, CVT_INT, CVT_REAL, CVT_COLOR, CVT_STRING};
		ValueType type;
		int ival;
		double rval;
		Uint32 cval;
		std::string sval;
		ConfigValue(const std::string& value = "");
		ConfigValue operator+(const ConfigValue& right);
		ConfigValue operator-(const ConfigValue& right);
		ConfigValue operator/(const int& right);
	};
	using configMap = std::map<std::string, std::map<std::string, ConfigValue>>;
	static configMap config;
	static std::map<std::string, SDL_Rect> rectMap;
	static bool loaded;
	static void loadConfig();
	Config();
};

