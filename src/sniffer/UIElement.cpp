#include "UIElement.h"

UIElement::UIElement(SDL_Rect rect, PState state)
    : mRect(rect), mActiveState(state), mPoint{0, 0}, mConf(Config::getInstance()) {}

UIElement::~UIElement() {}

bool UIElement::isClicked(const SDL_MouseButtonEvent e, PState &state) {
  // could use SDL_PointInRect
  mPoint.x = e.x;
  mPoint.y = e.y;
  return SDL_PointInRect(&mPoint, &mRect);
}

bool UIElement::isActive(const PState &state) const { return mActiveState == state; }
