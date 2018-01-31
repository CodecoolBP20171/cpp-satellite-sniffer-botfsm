#include "stdafx.h"
#include "Map.h"
#include "Resources.h"
#include "Config.h"
#include "Globals.h"

Map::Map(SDL_Rect rect, PState state)
	: UIElement(rect, state),
	map(Resources::getInstance()->getMap()),
	cleanMap(Resources::getInstance()->getCleanMap()),
	zoom{0}
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
		if (e.button == SDL_BUTTON_LEFT) {
			if (zoom < 3) {
				//TODO: clamping at border
				zoom++;
				mouseX = e.x * source.w / rect.w + source.x;
				mouseY = (e.y - Config::getIntOption("Dimensions", "MENU_HEIGHT")) * source.h / rect.h + source.y;
				source.w = (map->getDimensions().w / std::pow(2, zoom));
				source.h = (map->getDimensions().h / std::pow(2, zoom));
				source.x = mouseX - source.w / 2;
				source.y = mouseY - source.h / 2;
			}

		}
		if (e.button == SDL_BUTTON_RIGHT) {
			if (zoom > 0) {
				//TODO: recalculat for clamping at border
				zoom--;
				source.x = source.x - source.w / 2;
				source.y = source.y - source.h / 2;
				source.w = (map->getDimensions().w / std::pow(2, zoom));
				source.h = (map->getDimensions().h / std::pow(2, zoom));
			}
		}

		return true;
	}
	return false;
}

