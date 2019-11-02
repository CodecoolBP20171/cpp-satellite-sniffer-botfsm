#pragma once
#include <SDL.h>
#include "ProgramState.h"

class UIElement
{
public:
	UIElement(SDL_Rect rect, PState state);
	virtual ~UIElement();
	virtual bool isClicked(const SDL_MouseButtonEvent e, PState& state);
	virtual void render() = 0;
	bool isActive(const PState& state) const;
protected:
	SDL_Point point;
	SDL_Rect rect;
	const PState activeState;
};

