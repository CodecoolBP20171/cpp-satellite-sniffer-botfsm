#pragma once

#include "types.h"

#include <SDL.h>
#include <rapidjson/document.h>

#include <map>
#include <memory>
#include <string>
#include <string_view>

class Config {
public:
  static Config &getInstance();
  int getIntValue(const std::string_view &path);
  double getRealValue(const std::string_view &path);
  const Graphics::rgba_color &getColor(const std::string_view &path);
  std::string_view getStringValue(const std::string_view &path);
  const SDL_Rect &getRect(const std::string_view &name);

private:
  rapidjson::Document mConfig;
  std::map<std::string_view, SDL_Rect> rects;
  std::map<std::string_view, Graphics::rgba_color> colors;
  Config();
  void CalculateDimensions();
  void LoadSettingsFile();
  void CreateRects();
};
