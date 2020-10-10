#pragma once
#include "Config.h"

#include <CoordGeodetic.h>
#include <SDL.h>

#include <memory>
#include <vector>

class Satellite;
class Trajectory {
public:
  enum Direction { FORWARD = 1, BACK = -1 };
  explicit Trajectory(Direction direction);
  void calculate(std::time_t time, Satellite &sat);
  void render(int zoom);

private:
  Config &mConf;
  bool isTextureValid;
  SDL_Rect rect{};
  Direction direction;
  std::vector<CoordGeodetic> points;
  std::vector<float> drawPoints; // coordinate pairs normalized to 0..1
  double getDistance(const CoordGeodetic &a, const CoordGeodetic &b);
  void renderNewTexture([[maybe_unused]] int zoom);
  int lastZoom;
};
