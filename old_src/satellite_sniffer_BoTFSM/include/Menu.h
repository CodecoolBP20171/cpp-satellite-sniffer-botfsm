#pragma once
#include <memory>
#include <list>
#include "UIElement.h"
#include "Sprite.h"

class Menu : public UIElement
{
public:
	Menu(SDL_Rect rect, PState state, PState& programState);
	bool isClicked(const SDL_MouseButtonEvent e, PState& state) override;
	void render() override;
private:
	PState& state;
};

