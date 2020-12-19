#include "Config.h"

#include <rapidjson/pointer.h>
#include <spdlog/fmt/fmt.h>

#include <charconv>
#include <fstream>
#include <streambuf>
#include <string>

const SDL_Rect &Config::getRect(const std::string_view &name) {
  if (rects.find(name) != rects.end()) { return rects.at(name); }
  throw std::runtime_error(fmt::format("No rect found with name: {}", name.data()));
}

Config::Config() {
  LoadSettingsFile();
  CalculateDimensions();
  CreateRects();
}

void Config::CreateRects() {
  rects["MENU"] = SDL_Rect{getIntValue("/Dimensions/ORIGO_X"),
                           getIntValue("/Dimensions/ORIGO_Y"),
                           getIntValue("/Dimensions/WINDOW_WIDTH"),
                           getIntValue("/Dimensions/MENU_HEIGHT")};
  rects["POPUP"] = SDL_Rect{getIntValue("/Dimensions/POPUP_OFFSET_X"),
                            getIntValue("/Dimensions/POPUP_OFFSET_Y"),
                            getIntValue("/Dimensions/POPUP_WIDTH"),
                            getIntValue("/Dimensions/POPUP_HEIGHT")};
  rects["MAP"] = SDL_Rect{getIntValue("/Dimensions/ORIGO_X"),
                          getIntValue("/Dimensions/MENU_HEIGHT"),
                          getIntValue("/Dimensions/MAP_WIDTH"),
                          getIntValue("/Dimensions/MAP_HEIGHT")};
}

void Config::LoadSettingsFile() {
  std::ifstream confFile("settings.json");
  std::string confCont;
  confFile.seekg(0, std::ios::end);
  confCont.reserve(confFile.tellg());
  confFile.seekg(0, std::ios::beg);
  confCont.assign(std::istreambuf_iterator<char>(confFile), std::istreambuf_iterator<char>());
  const auto flags{rapidjson::kParseDefaultFlags | rapidjson::kParseCommentsFlag | // NOLINT(hicpp-signed-bitwise)
                   rapidjson::kParseTrailingCommasFlag};                           // NOLINT(hicpp-signed-bitwise)
  mConfig.Parse<flags>(confCont.c_str());
}

void Config::CalculateDimensions() {
  auto winW{getIntValue("/Dimensions/MAP_WIDTH")};
  auto winH{getIntValue("/Dimensions/MAP_HEIGHT") + getIntValue("/Dimensions/MENU_HEIGHT")};
  auto popW{winW - getIntValue("/Dimensions/MENU_BUTTON_WIDTH")};
  auto popH{getIntValue("/Dimensions/MAP_HEIGHT")};
  auto menuButVerSpace{(getIntValue("/Dimensions/MENU_HEIGHT") - getIntValue("/Dimensions/MENU_BUTTON_HEIGHT")) / 2};
  auto popOffX{(winW - popW) / 2};
  auto popOffY{(winH - popH) / 2};

  auto dimensions{rapidjson::Pointer("/Dimensions").Get(mConfig)};

  dimensions->AddMember("WINDOW_WIDTH", winW, mConfig.GetAllocator());
  dimensions->AddMember("WINDOW_HEIGHT", winH, mConfig.GetAllocator());
  dimensions->AddMember("POPUP_WIDTH", popW, mConfig.GetAllocator());
  dimensions->AddMember("POPUP_HEIGHT", popH, mConfig.GetAllocator());
  dimensions->AddMember("MENU_BUTTON_VERTICAL_SPACING", menuButVerSpace, mConfig.GetAllocator());
  dimensions->AddMember("POPUP_OFFSET_X", popOffX, mConfig.GetAllocator());
  dimensions->AddMember("POPUP_OFFSET_Y", popOffY, mConfig.GetAllocator());
}

Config &Config::getInstance() {
  static Config instance;
  return instance;
}

int Config::getIntValue(const std::string_view &path) {
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsInt()) { return val->GetInt(); }
  throw std::runtime_error(fmt::format("No integer value at path: {}", path.data()));
}

double Config::getRealValue(const std::string_view &path) {
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsDouble()) { return val->GetDouble(); }
  throw std::runtime_error(fmt::format("No floating point value at path: {}", path.data()));
}

const Graphics::rgba_color &Config::getColor(const std::string_view &path) {
  if (colors.find(path) != colors.end()) { return colors.at(path); }
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsString() && val->GetStringLength() == 8) {
    std::string_view strVal{val->GetString()};
    auto R{strVal.substr(0, 2)};
    auto G{strVal.substr(2, 2)};
    auto B{strVal.substr(4, 2)};
    auto A{strVal.substr(6, 2)};
    int r, g, b, a;
    auto rr = std::from_chars(R.data(), R.data() + R.size(), r, 16);
    auto gr = std::from_chars(G.data(), G.data() + G.size(), g, 16);
    auto br = std::from_chars(B.data(), B.data() + B.size(), b, 16);
    auto ar = std::from_chars(A.data(), A.data() + A.size(), a, 16);
    static std::errc ok;
    if (rr.ec != ok || gr.ec != ok || br.ec != ok || ar.ec != ok) {
      throw std::runtime_error(fmt::format("Color parsing error at path: {}", path.data()));
    }
    colors[path] = Graphics::rgba_color{static_cast<float>(r) / 255.f,
                                        static_cast<float>(g) / 255.f,
                                        static_cast<float>(b) / 255.f,
                                        static_cast<float>(a) / 255.f};
    return colors[path];
  }
  throw std::runtime_error(fmt::format("No color value at path: {}", path.data()));
}

std::string_view Config::getStringValue(const std::string_view &path) {
  auto val{rapidjson::Pointer(path.data()).Get(mConfig)};
  if (val->IsString()) { return val->GetString(); }
  throw std::runtime_error(fmt::format("No string value at path: {}", path.data()));
}
