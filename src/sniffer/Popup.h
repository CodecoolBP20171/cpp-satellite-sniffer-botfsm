#pragma once
#include "UIElement.h"

#include <list>
#include <memory>

class Popup : public UIElement {
public:
  Popup(SDL_Rect rect, PState state, PState &programState);
  bool isClicked(const SDL_MouseButtonEvent &e, PState &state_) override;
  void render() override;

private:
  PState &state;
};
