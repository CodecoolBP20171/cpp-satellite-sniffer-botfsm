#include "stdafx.h"
#include "UIElement.h"


UIElement::UIElement(SDL_Rect rect, PState state) : rect(rect), activeState(state)
{
}


UIElement::~UIElement()
{
}

bool UIElement::isClicked(const int x, const int y, PState& state)
{
	return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}

bool UIElement::isActive(const PState & state) const
{
	return activeState == state;
}
