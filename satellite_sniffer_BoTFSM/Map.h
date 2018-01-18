#pragma once
#include <memory>
#include <list>
#include "Sprite.h"
#include "UIElement.h"
#include "UISatellite.h"

class Map :	public UIElement
{
public:
	Map(SDL_Rect rect, PState state, std::list<UISatellite>& UISats);
	void render();
private:
	std::list<UISatellite>& UISats;
	std::shared_ptr<Sprite> map;
	std::shared_ptr<Sprite> cleanMap;
};

