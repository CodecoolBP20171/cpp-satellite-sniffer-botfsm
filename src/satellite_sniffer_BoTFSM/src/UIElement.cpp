#include "stdafx.h"
#include "UIElement.h"


UIElement::UIElement(SDL_Rect rect, PState state) 
	: rect(rect), activeState(state), point{0,0}
{
}


UIElement::~UIElement()
{
}

bool UIElement::isClicked(const SDL_MouseButtonEvent e, PState& state)
{
	// could use SDL_PointInRect
	point.x = e.x; point.y = e.y;
	return SDL_PointInRect(&point, &rect);
}

bool UIElement::isActive(const PState & state) const
{
	return activeState == state;
}
