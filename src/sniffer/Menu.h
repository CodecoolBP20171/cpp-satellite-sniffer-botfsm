#pragma once
#include "UIElement.h"

#include <list>
#include <memory>

class Menu : public UIElement {
public:
  Menu(SDL_Rect rect, PState state, PState &programState);
  bool isClicked(const SDL_MouseButtonEvent &e, PState &state_) override;
  void render() override;

private:
  PState &state;
};
