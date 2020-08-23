#include "UISatellite.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"
#include "stdafx.h"

#include <SDL2_gfxPrimitives.h>

#include <list>

UISatellite::UISatellite(Satellite &sat)
    : UIElement({0, 0, 0, 0}, PState::RUNNING), sat(sat), trajectoryForward(sat.getForwardTrajectory()),
      trajectoryBackward(sat.getBackTrajectory()) {}

void UISatellite::render(int zoom) {
  if (!sat.isShown()) return;
  renderTrajectory(zoom);
  auto res{Resources::getInstance()};
  auto mapSize{res->mapDimensions};
  auto satSize{res->iconDimensions};
  auto satpos{sat.getPosition()};
  zoom = std::pow(2, zoom);
  float cx{static_cast<float>((satpos.longitude / (MathConstants::PI * 2) * mapSize.w) / mapSize.w)};
  float cy{static_cast<float>((satpos.latitude / MathConstants::PI * mapSize.h) / mapSize.h)};
  float hw{satSize.w / (mapSize.w * 2.f * zoom)};
  float hh{satSize.h / (mapSize.h * 2.f * zoom)};
  auto start{res->iconBuffer.size()};
  // TODO modify texture coords according to sat type
  res->iconBuffer.emplace_back(Resources::texture_vertex{cx - hw, cy + hh, 0.f, .25f});
  res->iconBuffer.emplace_back(Resources::texture_vertex{cx + hw, cy + hh, .25f, .25f});
  res->iconBuffer.emplace_back(Resources::texture_vertex{cx - hw, cy - hh, 0.f, 0.f});
  res->iconBuffer.emplace_back(Resources::texture_vertex{cx + hw, cy - hh, .25f, 0.f});
  res->iconIndexBuf.emplace_back(start + 0);
  res->iconIndexBuf.emplace_back(start + 1);
  res->iconIndexBuf.emplace_back(start + 2);
  res->iconIndexBuf.emplace_back(start + 2);
  res->iconIndexBuf.emplace_back(start + 1);
  res->iconIndexBuf.emplace_back(start + 3);
  // TODO render text with imgui
  // auto textPos(text->getDimensions());
  // textPos.x = satRect.x + satSize.w / zoom;
  // textPos.y = satRect.y;
  // textPos.w /= zoom;
  // textPos.h /= zoom;
  // text->render(&textPos);
}

bool UISatellite::isClicked(const SDL_MouseButtonEvent e, PState &state) { return false; }

void UISatellite::renderTrajectory(int zoom) {
  trajectoryForward.render(zoom);
  trajectoryBackward.render(zoom);
}
