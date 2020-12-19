#pragma once
#include "Satellite.h"
#include "Trajectory.h"
#include "UIElement.h"

#include <ctime>
#include <memory>

class UISatellite : UIElement {
public:
  explicit UISatellite(Satellite &sat);
  void render() override{};
  void render(int zoom_level);
  bool isClicked(const SDL_MouseButtonEvent &e, PState &state) override;

private:
  Satellite &sat;
  Trajectory &trajectoryForward;
  Trajectory &trajectoryBackward;
  void renderTrajectory(int zoom);
};
