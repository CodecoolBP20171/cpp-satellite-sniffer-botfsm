#pragma once
#include "Config.h"
#include "ProgramState.h"

#include <SDL.h>

class UIElement {
public:
  UIElement(SDL_Rect rect, PState state);
  virtual ~UIElement() = default;
  virtual bool isClicked(const SDL_MouseButtonEvent &e, PState &state);
  virtual void render() = 0;
  [[nodiscard]] bool isActive(const PState &state) const;

protected:
  SDL_Point mPoint;
  SDL_Rect mRect;
  const PState mActiveState;
  Config &mConf;
};
