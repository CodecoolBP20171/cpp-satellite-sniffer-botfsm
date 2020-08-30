#include "Config.h"

#include <rapidjson/pointer.h>

#include <exception>
#include <fstream>
#include <streambuf>
#include <string>

const SDL_Rect &Config::getRect(const std::string_view &name) {
  if (rects.find(name) != rects.end()) { return rects.at(name); }
  throw std::runtime_error(std::string("No rect found with name: ") + name.data());
}

Config::Config() {
  std::ifstream confFile("settings.json");
  std::string confCont;
  confFile.seekg(0, std::ios::end);
  confCont.reserve(confFile.tellg());
  confFile.seekg(0, std::ios::beg);
  confCont.assign(std::istreambuf_iterator<char>(confFile), std::istreambuf_iterator<char>());
  mConfig.Parse(confCont.c_str());
  // TODO fill Dimensions object
  // TODO create rects
}

Config &Config::getInstance() {
  static Config instance;
  return instance;
}

int Config::getIntValue(const std::string_view &path) {
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsInt()) { return val->GetInt(); }
  throw std::runtime_error(std::string("No integer value at path: ") + path.data());
}

double Config::getRealValue(const std::string_view &path) {
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsDouble()) { return val->GetDouble(); }
  throw std::runtime_error(std::string("No floating point value at path: ") + path.data());
}

const Graphics::rgba_color &Config::getColorValue(const std::string_view &path) {
  if (colors.find(path) != colors.end()) { return colors.at(path); }
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsString() && val->GetStringLength() == 8) {
    std::string_view strVal{val->GetString()};
    auto R{strVal.substr(0, 2)};
    auto G{strVal.substr(2, 2)};
    auto B{strVal.substr(4, 2)};
    auto A{strVal.substr(6, 2)};
    try {
      auto r{std::stoi(R.data(), nullptr, 16)};
      auto g{std::stoi(G.data(), nullptr, 16)};
      auto b{std::stoi(B.data(), nullptr, 16)};
      auto a{std::stoi(A.data(), nullptr, 16)};
      colors[path] = Graphics::rgba_color{r / 255.f, g / 255.f, b / 255.f, a / 255.f};
    } catch (std::exception &) { throw std::runtime_error(std::string("Color parsing error at path: ") + path.data()); }
  }
  throw std::runtime_error(std::string("No color value at path: ") + path.data());
}

const std::string_view &Config::getStringValue(const std::string_view &path) {
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsString()) { return val->GetString(); }
  throw std::runtime_error(std::string("No string value at path: ") + path.data());
}
