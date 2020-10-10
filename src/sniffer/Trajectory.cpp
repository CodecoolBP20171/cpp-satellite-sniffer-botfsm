#include "Trajectory.h"

#include "Config.h"
#include "Globals.h"
#include "Resources.h"
#include "Satellite.h"

Trajectory::Trajectory(Direction direction)
    : mConf(Config::getInstance()), direction(direction), isTextureValid(false), lastZoom(0) {}

void Trajectory::calculate(std::time_t time, Satellite &sat) {
  points.clear();

  int cntr(0);
  double fullDistance(0.0);
  static const double MAX_DISTANCE = mConf.getRealValue("/TrajectoryLimits/MAX_DISTANCE");
  static const int MAX_POINTS = mConf.getIntValue("/TrajectoryLimits/MAX_POINTS");

  do {
    points.emplace_back(sat.getPositionAtTime(time));
    time += direction * sat.getDelta(time);
    if (points.size() >= 2) fullDistance += getDistance(points[cntr], points[cntr - 1]);
    cntr++;
  } while (points.size() < 2 || (fullDistance < MAX_DISTANCE && cntr < MAX_POINTS));

  isTextureValid = false;
}

void Trajectory::render(int zoom) {
  if (!isTextureValid || lastZoom != zoom) {}
  renderNewTexture(zoom);
  isTextureValid = true;
  lastZoom = zoom;
}

void Trajectory::renderNewTexture([[maybe_unused]] int zoom) {
  auto res = Resources::getInstance();
  auto [R, G, B, A]{mConf.getColorValue(direction == Direction::FORWARD ? "/TrajectoryRender/FORWARD_COLOR"
                                                                        : "/TrajectoryRender/BACKWARD_COLOR")};
  auto lastX{static_cast<float>(points[0].longitude / (MathConstants::PI * 2))};
  res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
  res->trajectoryBuffer.emplace_back(
      Graphics::color_vertex{lastX, static_cast<float>(points[0].latitude / MathConstants::PI), R, G, B});
  for (int i = 1; i < points.size(); ++i) {
    auto x{static_cast<float>(points[i].longitude / (MathConstants::PI * 2))};
    auto y{static_cast<float>(points[i].latitude / MathConstants::PI)};
    float delta = abs(x - lastX);
    if (delta > mConf.getRealValue("/TrajectoryLimits/RENDER_DISTANCE_GAP")) {
      if (x > lastX) {
        x = 0.f;
        res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
        res->trajectoryBuffer.emplace_back(Graphics::color_vertex{x, y, R, G, B});
        x = 1.f;
      } else {
        x = 1.f;
        res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
        res->trajectoryBuffer.emplace_back(Graphics::color_vertex{x, y, R, G, B});
        x = 0.f;
      }
      res->trajectoryIndexBuf.emplace_back(-1);
    }
    res->trajectoryIndexBuf.emplace_back(res->trajectoryBuffer.size());
    res->trajectoryBuffer.emplace_back(Graphics::color_vertex{x, y, R, G, B});

    lastX = x;
  }
  res->trajectoryIndexBuf.emplace_back(-1);
}

double Trajectory::getDistance(const CoordGeodetic &a, const CoordGeodetic &b) {
  double latDelta = a.latitude - b.latitude;
  double longDelta = a.longitude - b.longitude;
  double distGap = mConf.getRealValue("/TrajectoryLimits/DISTANCE_GAP");
  if (abs(longDelta) > distGap) longDelta = distGap;
  return sqrt(pow(latDelta, 2) + pow(longDelta, 2));
}
