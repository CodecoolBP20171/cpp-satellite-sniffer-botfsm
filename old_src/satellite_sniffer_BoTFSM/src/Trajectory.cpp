#include "Trajectory.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"
#include "Satellite.h"
#include "stdafx.h"

#include <SDL2_gfxPrimitives.h>

Trajectory::Trajectory(Direction direction) : direction(direction), isTextureValid(false), lastZoom(0) {}

void Trajectory::calculate(std::time_t time, Satellite &sat) {
  points.clear();

  int cntr(0);
  double fullDistance(0.0);
  static const double MAX_DISTANCE = Config::getRealOption("TrajectoryLimits", "MAX_DISTANCE");
  static const int MAX_POINTS = Config::getIntOption("TrajectoryLimits", "MAX_POINTS");

  do {
    points.emplace_back(sat.getPositionAtTime(time));
    time += direction * sat.getDelta(time);
    if (points.size() >= 2) fullDistance += getDistance(points[cntr], points[cntr - 1]);
    cntr++;
  } while (points.size() < 2 || (fullDistance < MAX_DISTANCE && cntr < MAX_POINTS));

  isTextureValid = false;
}

void Trajectory::render(int zoom) {
  if (!isTextureValid || lastZoom != zoom) {
    // updateRect(zoom);
    // texture.resize(rect.w, rect.h);
  }
  renderNewTexture(zoom);
  isTextureValid = true;
  lastZoom = zoom;
  // texture.render(&rect);
}

void Trajectory::renderNewTexture(int zoom) {
  // texture.setAsRenderTarget();
  auto res = Resources::getInstance();
  float lastX = points[0].longitude / (MathConstants::PI * 2);
  res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
  res->trajectoryBuffer.emplace_back(
      Resources::color_vertex{lastX, static_cast<float>(points[0].latitude / MathConstants::PI), 1.f, 1.f, 0.f});
  for (int i = 1; i < points.size(); ++i) {
    float x(points[i].longitude / (MathConstants::PI * 2));
    float y(points[i].latitude / MathConstants::PI);
    float delta = abs(x - lastX);
    if (delta > Config::getRealOption("TrajectoryLimits", "RENDER_DISTANCE_GAP")) {
      if (x > lastX) {
        x = 0.f;
        res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
        // TODO set color depending on forward/backward trajectory
        res->trajectoryBuffer.emplace_back(Resources::color_vertex{x, y, 1.f, 1.f, 0.f});
        x = 1.f;
      } else {
        x = 1.f;
        res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
        // TODO set color depending on forward/backward trajectory
        res->trajectoryBuffer.emplace_back(Resources::color_vertex{x, y, 1.f, 1.f, 0.f});
        x = 0.f;
      }
      res->trajectoryIndexBuf.emplace_back(-1);
    }
    res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
    // TODO set color depending on forward/backward trajectory
    res->trajectoryBuffer.emplace_back(Resources::color_vertex{x, y, 1.f, 1.f, 0.f});

    lastX = x;
  }
  res->trajectoryIndexBuf.emplace_back(-1);
}

void Trajectory::renderSegment(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, int zoom) {
  x1 -= rect.x;
  x2 -= rect.x;
  y1 -= rect.y;
  y2 -= rect.y;
  Uint32 color(FORWARD == direction ? Config::getColorOption("TrajectoryRender", "FORWARD_COLOR")
                                    : Config::getColorOption("TrajectoryRender", "BACKWARD_COLOR"));
  thickLineColor(Resources::getInstance()->getRenderer(),
                 x1,
                 y1,
                 x2,
                 y2,
                 Config::getIntOption("TrajectoryRender", "LINE_WEIGHT") / std::pow(2, zoom),
                 color);
}

double Trajectory::getDistance(const CoordGeodetic &a, const CoordGeodetic &b) {
  double latDelta = a.latitude - b.latitude;
  double longDelta = a.longitude - b.longitude;
  double distGap = Config::getRealOption("TrajectoryLimits", "DISTANCE_GAP");
  if (abs(longDelta) > distGap) longDelta = distGap;
  return sqrt(pow(latDelta, 2) + pow(longDelta, 2));
}

void Trajectory::updateRect(int zoom) {
  auto mapSize(Resources::getInstance()->getMapDimensions());
  int minX{mapSize.w}, maxX{0}, minY{mapSize.h}, maxY{0};
  for (int i = 0; i < points.size(); ++i) {
    Sint16 pX(static_cast<Sint16>(round(points[i].longitude / (MathConstants::PI * 2) * mapSize.w)));
    Sint16 pY(static_cast<Sint16>(round(points[i].latitude / (MathConstants::PI)*mapSize.h)));
    if (pX < minX) minX = pX;
    if (pY < minY) minY = pY;
    if (pX > maxX) maxX = pX;
    if (pY > maxY) maxY = pY;
  }
  int lineWeight = Config::getIntOption("TrajectoryRender", "LINE_WEIGHT") / std::pow(2, zoom);
  rect.x = minX - lineWeight;
  rect.y = minY - lineWeight;
  rect.w = maxX - rect.x + 2 * lineWeight;
  rect.h = maxY - rect.y + 2 * lineWeight;
  if (rect.x < 2 * lineWeight) { rect.x = 0; }
  if (rect.y < 2 * lineWeight) { rect.y = 0; }
  if (rect.w > mapSize.w - 2 * lineWeight) { rect.w = mapSize.w; }
  if (rect.h > mapSize.h - 2 * lineWeight) { rect.h = mapSize.h; }
}
