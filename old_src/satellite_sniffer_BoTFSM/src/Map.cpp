#include "Map.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"
#include "stdafx.h"

Map::Map(SDL_Rect rect, PState state, int &zoom)
    : UIElement(rect, state), source(Resources::getInstance()->mapDimensions), zoom(zoom) {}

void Map::render() {
  auto res = Resources::getInstance();
  res->zcx = (source.x + .5f * source.w) / res->mapDimensions.w;
  res->zcy = (source.y + .5f * source.h) / res->mapDimensions.h;

  glViewport(0, 0, rect.w, rect.h);

  glUseProgram(res->textureProgramId);
  glBindTexture(GL_TEXTURE_2D, res->mapTextureId);
  // TODO save uniform location
  glUniform1i(glGetUniformLocation(res->textureProgramId, "tex_sampl"), 0);
  glUniform2f(glGetUniformLocation(res->textureProgramId, "zoom_center"), res->zcx, res->zcy);
  glUniform1f(glGetUniformLocation(res->textureProgramId, "zoom_level"), static_cast<float>(std::pow(2, zoom)));

  glBindVertexArray(res->mapVAOId);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);

  GL_CHECK;

  Satellites::getInstance()->renderUISatellits(zoom);
}

bool Map::isClicked(const SDL_MouseButtonEvent e, PState &state) {
  if (UIElement::isClicked(e, state)) {
    if (e.button == SDL_BUTTON_LEFT && zoom < Config::getIntOption("ZoomLevel", "MAX")) { zoomIn(e); }
    if (e.button == SDL_BUTTON_RIGHT && zoom > Config::getIntOption("ZoomLevel", "MIN")) { zoomOut(); }
    return true;
  }
  return false;
}

void Map::zoomIn(const SDL_MouseButtonEvent &e) {
  auto res = Resources::getInstance();
  zoom++;
  source.x = e.x * source.w / rect.w + source.x;
  source.y = (e.y - Config::getIntOption("Dimensions", "MENU_HEIGHT")) * source.h / rect.h + source.y;
  source.w = (res->mapDimensions.w / std::pow(2, zoom));
  source.h = (res->mapDimensions.h / std::pow(2, zoom));
  source.x -= source.w / 2;
  source.y -= source.h / 2;
  clampToBorder();
}

void Map::zoomOut() {
  auto res = Resources::getInstance();
  zoom--;
  source.x -= source.w / 2;
  source.y -= source.h / 2;
  source.w = (res->mapDimensions.w / std::pow(2, zoom));
  source.h = (res->mapDimensions.h / std::pow(2, zoom));
  clampToBorder();
}

void Map::clampToBorder() {
  auto res = Resources::getInstance();
  if (source.x <= 0) {
    source.x = 0;
  } else if (source.x + source.w > res->mapDimensions.w) {
    source.x = res->mapDimensions.w - source.w;
  }
  if (source.y <= 0) {
    source.y = 0;
  } else if (source.y + source.h > res->mapDimensions.h) {
    source.y = res->mapDimensions.h - source.h;
  }
}
