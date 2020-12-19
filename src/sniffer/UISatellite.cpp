#include "UISatellite.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"

#include <SDL2_gfxPrimitives.h>

#include <imgui.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
UISatellite::UISatellite(Satellite &sat)
    : UIElement({0, 0, 0, 0}, PState::RUNNING), sat(sat), trajectoryForward(sat.getForwardTrajectory()),
      trajectoryBackward(sat.getBackTrajectory()) {}

void UISatellite::render(int zoom) {
  if (!sat.isShown()) return;
  auto &res{Resources::getInstance()};
  auto zoomRect{res.mapDimensions};

  renderTrajectory(zoom);
  auto mapSize{res.mapDimensions};
  auto satSize{res.iconDimensions};
  auto satpos{sat.getPosition()};
  auto zoom_level{std::pow(2, zoom)};
  float cx{static_cast<float>((satpos.longitude / (MathConstants::PI * 2) * mapSize.w) / mapSize.w)};
  float cy{static_cast<float>((satpos.latitude / MathConstants::PI * mapSize.h) / mapSize.h)};
  float hw{static_cast<float>(satSize.w) / (static_cast<float>(mapSize.w) * 2.f * static_cast<float>(zoom_level))};
  float hh{static_cast<float>(satSize.h) / (static_cast<float>(mapSize.h) * 2.f * static_cast<float>(zoom_level))};
  auto start{res.iconBuffer.size()};
  const auto &textureVertices{res.getIconTextureVertices(sat.getType())};
  res.iconBuffer.emplace_back(
      Graphics::texture_vertex{cx - hw, cy + hh, textureVertices[0].texx, textureVertices[0].texy});
  res.iconBuffer.emplace_back(
      Graphics::texture_vertex{cx + hw, cy + hh, textureVertices[1].texx, textureVertices[1].texy});
  res.iconBuffer.emplace_back(
      Graphics::texture_vertex{cx - hw, cy - hh, textureVertices[2].texx, textureVertices[2].texy});
  res.iconBuffer.emplace_back(
      Graphics::texture_vertex{cx + hw, cy - hh, textureVertices[3].texx, textureVertices[3].texy});
  res.iconIndexBuf.emplace_back(start + 0);
  res.iconIndexBuf.emplace_back(start + 1);
  res.iconIndexBuf.emplace_back(start + 2);
  res.iconIndexBuf.emplace_back(start + 2);
  res.iconIndexBuf.emplace_back(start + 1);
  res.iconIndexBuf.emplace_back(start + 3);

  // TODO render text with imgui
  // auto textPos(text->getDimensions());
  // textPos.x = satRect.x + satSize.w / zoom;
  // textPos.y = satRect.y;
  // textPos.w /= zoom;
  // textPos.h /= zoom;
  // text->render(&textPos);
  ImGui::SetNextWindowContentSize({static_cast<float>(mConf.getIntValue("/Dimensions/MAP_WIDTH")),
                                   static_cast<float>(mConf.getIntValue("/Dimensions/MAP_HEIGHT"))});
  ImGui::SetNextWindowPos({0.f, static_cast<float>(mConf.getIntValue("/Dimensions/MENU_HEIGHT"))});
  auto textSize{ImGui::CalcTextSize(sat.getName().c_str())};
  if (ImGui::Begin("name_overlay",
                   nullptr,
                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs)) {
    ImGui::SetCursorPos({0.f, 0.f});
    ImGui::Text("%s", sat.getName().c_str());
    ImGui::End();
  }
}

bool UISatellite::isClicked(const SDL_MouseButtonEvent &e, PState &state) { return false; }

void UISatellite::renderTrajectory(int zoom) {
  trajectoryForward.render(zoom);
  trajectoryBackward.render(zoom);
}

#pragma clang diagnostic pop