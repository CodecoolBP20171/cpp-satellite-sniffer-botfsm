#pragma once
#include <SDL.h>

class UIElement
{
public:
	UIElement(SDL_Rect rect);
	virtual ~UIElement();
	virtual bool isClicked(const int x, const int y);
	virtual void render() = 0;
protected:
	SDL_Rect rect;
};

