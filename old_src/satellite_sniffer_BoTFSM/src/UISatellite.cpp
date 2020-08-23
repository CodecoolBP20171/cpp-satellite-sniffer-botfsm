#include "UISatellite.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"
#include "stdafx.h"

#include <SDL2_gfxPrimitives.h>

#include <list>

int UISatellite::pos{0};

UISatellite::UISatellite(Satellite &sat)
    : UIElement({0, 0, 0, 0}, PState::RUNNING), sat(sat), texture(Resources::getInstance()->getSat(sat.getType())),
      trajectoryForward(sat.getForwardTrajectory()), trajectoryBackward(sat.getBackTrajectory()),
      text(new ScreenText(sat.getName())), popupText(new ScreenText(sat.getName())) {
  rect = popupText->getDimensions();
  double oldH(rect.h);
  rect.h = Config::getIntOption("Dimensions", "POPUP_HEIGHT") / 10;
  rect.w = static_cast<int>(rect.w * (rect.h / oldH));
  rect.x =
      Config::getIntOption("Dimensions", "POPUP_OFFSET_X") + Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
  if (pos >= 10) { rect.x += Config::getIntOption("Dimensions", "POPUP_WIDTH") / 2; }
  rect.y = Config::getIntOption("Dimensions", "POPUP_OFFSET_Y") + rect.h * (pos % 10);
  text->setColor({0, 0, 0});
  ++pos;
}

void UISatellite::render(int zoom) {
  if (!sat.isShown()) return;
  renderTrajectory(zoom);
  auto res = Resources::getInstance();
  auto mapSize(res->getMapDimensions());
  auto satSize(texture->getDimensions());
  auto satpos(sat.getPosition());
  zoom = std::pow(2, zoom);
  float cx = (satpos.longitude / (MathConstants::PI * 2) * mapSize.w) / mapSize.w;
  float cy = (satpos.latitude / MathConstants::PI * mapSize.h) / mapSize.h;
  float hw = satSize.w / (mapSize.w * 2.f * zoom);
  float hh = satSize.h / (mapSize.h * 2.f * zoom);
  auto start = res->iconBuffer.size();
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
