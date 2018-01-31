#include "stdafx.h"
#include "Map.h"
#include "Resources.h"

Map::Map(SDL_Rect rect, PState state)
	: UIElement(rect, state),
	map(Resources::getInstance()->getMap()),
	cleanMap(Resources::getInstance()->getCleanMap())
{}

void Map::render(PState* state)
{
	// Keeping the map shown under the popup menu
	SDL_RenderClear(Resources::getInstance()->getRenderer());

	// cleaning map before rendering satelites on it
	map->setAsRenderTarget();
	cleanMap->render(nullptr);

	Satellites::getInstance()->renderUISatellits();

	// setting renderer to Window
	Resources::getInstance()->resetRenderer();
	map->render(&rect);
}

