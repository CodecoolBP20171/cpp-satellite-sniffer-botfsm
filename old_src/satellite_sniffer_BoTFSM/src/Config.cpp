#include "stdafx.h"
#include "Config.h"
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#include <INIParser.h>
#include <rapidjson/document.h>

Config::configMap Config::config = Config::configMap();
std::map<std::string, SDL_Rect> Config::rectMap = std::map<std::string, SDL_Rect>();
bool Config::loaded = false;

int Config::getIntOption(const std::string & section, const std::string & option)
{
	if (!loaded) Config::loadConfig();
	auto opt = Config::config[section][option];
	if (opt.type && opt.type == ConfigValue::CVT_INT) return opt.ival;
	return 0;
}

double Config::getRealOption(const std::string & section, const std::string & option)
{
	if (!loaded) Config::loadConfig();
	auto opt = Config::config[section][option];
	if (opt.type && opt.type == ConfigValue::CVT_REAL) return opt.rval;
	return 0.0;
}

Uint32 Config::getColorOption(const std::string & section, const std::string & option)
{
	if (!loaded) Config::loadConfig();
	auto opt = Config::config[section][option];
	if (opt.type && opt.type == ConfigValue::CVT_COLOR) return opt.cval;
	return Uint32();
}

std::string Config::getStringOption(const std::string & section, const std::string & option)
{
	if (!loaded) Config::loadConfig();
	auto opt = Config::config[section][option];
	if (opt.type && opt.type == ConfigValue::CVT_STRING) return opt.sval;
	return std::string();
}

const SDL_Rect & Config::getRect(const std::string & name)
{
	return rectMap[name];
}

void Config::loadConfig()
{
//    std::ifstream confFile("settings.json");
//    std::string confCont;
//    confFile.seekg(0,std::ios::end);
//    confCont.reserve(confFile.tellg());
//    confFile.seekg(0,std::ios::beg);
//    confCont.assign(std::istreambuf_iterator<char>(confFile),std::istreambuf_iterator<char>());
//    mConfig.Parse(confCont.c_str());

	cppiniparser::INIConfig cfg = cppiniparser::INIParser::Read("settings.ini");
	auto sections = cfg.Sections();
	for (auto& section : sections) {
		auto options = cfg.Options(section);
		for (auto& option : options) {
			Config::config[section][option] = ConfigValue(cfg.GetOption(section, option));
		}
	}
	auto& dimensions = Config::config["Dimensions"];
	dimensions["WINDOW_WIDTH"] = dimensions["MAP_WIDTH"];
	dimensions["WINDOW_HEIGHT"] = dimensions["MAP_HEIGHT"] + dimensions["MENU_HEIGHT"];
	dimensions["POPUP_WIDTH"] = dimensions["WINDOW_WIDTH"] - dimensions["MENU_BUTTON_WIDTH"];
	dimensions["POPUP_HEIGHT"] = dimensions["MAP_HEIGHT"];
	dimensions["MENU_BUTTON_VERTICAL_SPACING"] = (dimensions["MENU_HEIGHT"] - dimensions["MENU_BUTTON_HEIGHT"]) / 2;
	dimensions["POPUP_OFFSET_X"] = (dimensions["WINDOW_WIDTH"] - dimensions["POPUP_WIDTH"]) / 2;
	dimensions["POPUP_OFFSET_Y"] = (dimensions["WINDOW_HEIGHT"] - dimensions["POPUP_HEIGHT"]) / 2;
	rectMap["MENU"] = SDL_Rect{ dimensions["ORIGO_X"].ival, dimensions["ORIGO_Y"].ival,
								dimensions["WINDOW_WIDTH"].ival, dimensions["MENU_HEIGHT"].ival };
	rectMap["POPUP"] = SDL_Rect{ dimensions["POPUP_OFFSET_X"].ival, dimensions["POPUP_OFFSET_Y"].ival,
								dimensions["POPUP_WIDTH"].ival, dimensions["POPUP_HEIGHT"].ival };
	rectMap["MAP"] = SDL_Rect{ dimensions["ORIGO_X"].ival, dimensions["MENU_HEIGHT"].ival,
								dimensions["MAP_WIDTH"].ival, dimensions["MAP_HEIGHT"].ival };
	Config::loaded = true;
}

Config::Config(){}

Config::ConfigValue::ConfigValue(const std::string & value) :
	type(CVT_UNKNOWN),
	ival(0),
	rval(0.0)
{
	if (value.empty()) return;
	sval = value;
	type = CVT_STRING;
	if (sval.substr(0, 2) == "0x") {
		cval = stoul(value, 0, 16);
		type = CVT_COLOR;
		return;
	}
	try {
		rval = stod(value);
		type = CVT_REAL;
		if (value.find('.') != std::string::npos) return;
		ival = stoi(value);
		type = CVT_INT;
	}
	catch (...) {
		return;
	}
}

Config::ConfigValue Config::ConfigValue::operator+(const ConfigValue & right)
{
	ConfigValue res(*this);
	if (type == right.type) {
		switch (type) {
		case ConfigValue::CVT_INT:
			res.ival += right.ival;
			break;
		case ConfigValue::CVT_REAL:
			res.rval += right.rval;
			break;
		default:
			return ConfigValue();
		}
		return res;
	}
	return ConfigValue();
}

Config::ConfigValue Config::ConfigValue::operator-(const ConfigValue & right)
{
	ConfigValue res(*this);
	if (type == right.type) {
		switch (type) {
		case ConfigValue::CVT_INT:
			res.ival -= right.ival;
			break;
		case ConfigValue::CVT_REAL:
			res.rval -= right.rval;
			break;
		default:
			return ConfigValue();
		}
		return res;
	}
	return ConfigValue();
}

Config::ConfigValue Config::ConfigValue::operator/(const int & right)
{
	ConfigValue res(*this);
	switch (type) {
	case ConfigValue::CVT_INT:
		res.ival /= right;
		break;
	case ConfigValue::CVT_REAL:
		res.rval /= right;
		break;
	default:
		return ConfigValue();
	}
	return res;
}
