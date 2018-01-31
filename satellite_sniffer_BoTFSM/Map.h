#pragma once
#include <memory>
#include <list>
#include "Sprite.h"
#include "UIElement.h"
#include "Satellites.h"

class Map :	public UIElement
{
public:
	Map(SDL_Rect rect, PState state);
	void render(PState* state = nullptr) override;
private:
	std::shared_ptr<Sprite> map;
	std::shared_ptr<Sprite> cleanMap;
};

