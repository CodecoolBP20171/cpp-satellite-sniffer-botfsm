#include "stdafx.h"
#include "Map.h"
#include "Resources.h"

Map::Map(SDL_Rect rect, PState state, std::list<UISatellite>& UISats)
	: UIElement(rect, state),
	UISats(UISats),
	map(Resources::getInstance()->getMap()),
	cleanMap(Resources::getInstance()->getCleanMap())
{}

void Map::render()
{
	// Keeping the map shown under the popup menu
	SDL_RenderClear(Resources::getInstance()->getRenderer());

	// cleaning map before rendering satelites on it
	map->setAsRenderTarget();
	cleanMap->render(nullptr);

	for (auto& sat : UISats) {
		sat.render();
	}

	// setting renderer to Window
	Resources::getInstance()->resetRenderer();
	map->render(&rect);
}

