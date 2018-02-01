#pragma once
#include <memory>
#include <list>
#include "Sprite.h"
#include "UIElement.h"
#include "Satellites.h"

class Map :	public UIElement
{
public:
	Map(SDL_Rect rect, PState state, int& zoom);
	void render();
	bool isClicked(const SDL_MouseButtonEvent e, PState& state) override;

private:
	std::shared_ptr<Sprite> map;
	std::shared_ptr<Sprite> cleanMap;
	SDL_Rect source;
	int mouseX, mouseY;
	int& zoom;
};

