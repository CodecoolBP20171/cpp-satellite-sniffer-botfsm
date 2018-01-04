#pragma once
#include <SDL.h>
#include "ProgramState.h"

class UIElement
{
public:
	UIElement(SDL_Rect rect, PState state);
	virtual ~UIElement();
	virtual bool isClicked(const int x, const int y, PState& state);
	virtual void render() = 0;
	bool isActive(const PState& state) const;
protected:
	SDL_Rect rect;
	const PState activeState;
};

