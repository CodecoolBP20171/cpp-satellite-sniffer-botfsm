#include "Map.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"

Map::Map(SDL_Rect rect, PState state, int &zoom)
    : UIElement(rect, state), source(Resources::getInstance()->mapDimensions), zoom(zoom) {}

void Map::render() {
  auto res = Resources::getInstance();
  res->zcx = (source.x + .5f * source.w) / res->mapDimensions.w;
  res->zcy = (source.y + .5f * source.h) / res->mapDimensions.h;

  glViewport(0, 0, mRect.w, mRect.h);

  glUseProgram(res->textureProgramId);
  glBindTexture(GL_TEXTURE_2D, res->mapTextureId);

  // TODO move this to a better place
  static auto map_tsULoc{glGetUniformLocation(res->textureProgramId, "tex_sampl")};
  static auto map_zcULoc{glGetUniformLocation(res->textureProgramId, "zoom_center")};
  static auto map_zlULoc{glGetUniformLocation(res->textureProgramId, "zoom_level")};

  glUniform1i(map_tsULoc, 0);
  glUniform2f(map_zcULoc, res->zcx, res->zcy);
  glUniform1f(map_zlULoc, static_cast<float>(std::pow(2, zoom)));

  glBindVertexArray(res->mapVAOId);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);

  GL_CHECK;

  Satellites::getInstance()->renderUISatellits(zoom);
}

bool Map::isClicked(const SDL_MouseButtonEvent e, PState &state) {
  if (UIElement::isClicked(e, state)) {
    if (e.button == SDL_BUTTON_LEFT && zoom < mConf.getIntValue("/ZoomLevel/MAX")) { zoomIn(e); }
    if (e.button == SDL_BUTTON_RIGHT && zoom > mConf.getIntValue("/ZoomLevel/MIN")) { zoomOut(); }
    return true;
  }
  return false;
}

void Map::zoomIn(const SDL_MouseButtonEvent &e) {
  auto res = Resources::getInstance();
  zoom++;
  source.x = e.x * source.w / mRect.w + source.x;
  source.y = (e.y - mConf.getIntValue("/Dimensions/MENU_HEIGHT")) * source.h / mRect.h + source.y;
  source.w = static_cast<int>(std::ceil(res->mapDimensions.w / std::pow(2, zoom)));
  source.h = static_cast<int>(std::ceil(res->mapDimensions.h / std::pow(2, zoom)));
  source.x -= source.w / 2;
  source.y -= source.h / 2;
  clampToBorder();
}

void Map::zoomOut() {
  auto res = Resources::getInstance();
  zoom--;
  source.x -= source.w / 2;
  source.y -= source.h / 2;
  source.w = static_cast<int>(std::ceil(res->mapDimensions.w / std::pow(2, zoom)));
  source.h = static_cast<int>(std::ceil(res->mapDimensions.h / std::pow(2, zoom)));
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
