#include "stdafx.h"
#include "Map.h"
#include "Resources.h"
#include "Config.h"
#include "Globals.h"

Map::Map(SDL_Rect rect, PState state, int& zoom)
	: UIElement(rect, state),
	map(Resources::getInstance()->getMap()),
	cleanMap(Resources::getInstance()->getCleanMap()),
	zoom(zoom)
{
	source = map->getDimensions();
}

void Map::render()
{
	// Keeping the map shown under the popup menu
	SDL_RenderClear(Resources::getInstance()->getRenderer());

	// cleaning map before rendering satelites on it
	map->setAsRenderTarget();
	cleanMap->render(nullptr);

	Satellites::getInstance()->renderUISatellits(zoom);

	// setting renderer to Window
	Resources::getInstance()->resetRenderer();
	map->render(&source, &rect);
}

bool Map::isClicked(const SDL_MouseButtonEvent e, PState & state)
{
	if (UIElement::isClicked(e, state)) {
		if (e.button == SDL_BUTTON_LEFT 
			&& zoom < Config::getIntOption("ZoomLevel", "MAX")) 
		{
			zoomIn(e);
		}
		if (e.button == SDL_BUTTON_RIGHT 
			&& zoom > Config::getIntOption("ZoomLevel", "MIN")) 
		{
			zoomOut();
		}
		return true;
	}
	return false;
}

void Map::zoomIn(const SDL_MouseButtonEvent &e)
{
	zoom++;
	source.x = e.x * source.w / rect.w + source.x;
	source.y = (e.y - Config::getIntOption("Dimensions", "MENU_HEIGHT")) * source.h / rect.h + source.y;
	source.w = (map->getDimensions().w / std::pow(2, zoom));
	source.h = (map->getDimensions().h / std::pow(2, zoom));
	source.x -= source.w / 2;
	source.y -= source.h / 2;
	clampToBorder();
}

void Map::zoomOut()
{
	zoom--;
	source.x -= source.w / 2;
	source.y -= source.h / 2;
	source.w = (map->getDimensions().w / std::pow(2, zoom));
	source.h = (map->getDimensions().h / std::pow(2, zoom));
	clampToBorder();
}

void Map::clampToBorder()
{
	if (source.x <= 0) {
		source.x = 0;
	}
	else if (source.x + source.w > map->getDimensions().w) {
		source.x = map->getDimensions().w - source.w;
	}
	if (source.y <= 0) {
		source.y = 0;
	}
	else if (source.y + source.h >  map->getDimensions().h) {
		source.y = map->getDimensions().h - source.h;
	}
}
