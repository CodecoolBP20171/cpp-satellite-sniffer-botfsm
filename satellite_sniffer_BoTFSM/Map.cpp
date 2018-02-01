#include "stdafx.h"
#include "Map.h"
#include "Resources.h"
#include "Config.h"
#include "Globals.h"
#include <iostream> // for debugging

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
		if (e.button == SDL_BUTTON_LEFT) {
			if (zoom < 3) {
				zoom++;
				std::cout << "In Before Source: " << source.x << " " << source.y << "\n";
				mouseX = e.x * source.w / rect.w + source.x;
				mouseY = (e.y - Config::getIntOption("Dimensions", "MENU_HEIGHT")) * source.h / rect.h + source.y;
				source.w = (map->getDimensions().w / std::pow(2, zoom));
				source.h = (map->getDimensions().h / std::pow(2, zoom));
				
				if (mouseX - source.w / 2 < 0) {
					source.x = 0;
				}
				else if (mouseX + source.w / 2 > map->getDimensions().w) {
					source.x = mouseX - source.w / 2 - (mouseX + source.w / 2 - map->getDimensions().w);
				}
				else {
					source.x = mouseX - source.w / 2;
				}
				if (mouseY - source.h / 2 < 0) {
					source.y = 0;
				}
				else if (mouseY + source.h / 2 > map->getDimensions().h) {
					source.y = mouseY - source.h / 2 - (mouseY + source.h / 2 - map->getDimensions().h);
				}
				else {
					source.y = mouseY - source.h / 2;
				}
				std::cout << "In After Source: " << source.x << " " << source.y << "\n";
			}

		}
		if (e.button == SDL_BUTTON_RIGHT) {
			if (zoom > 0) {
				std::cout << "Out Before Source: " << source.x << " " << source.y << "\n";
				
				zoom--;
				int oldW = source.w;
				int oldH = source.w;
				source.w = (map->getDimensions().w / std::pow(2, zoom));
				source.h = (map->getDimensions().h / std::pow(2, zoom));
				if (source.x - oldW /2 <= 0) {
					source.x = 0;
				}
				else {
					source.x = source.x - oldW / 2;
				}
				if (source.y - oldH / 2 <= 0) {
					source.y = 0;
				}
				else {
					source.y = source.y - oldH / 2;
				}
				std::cout << "Out After Source: " << source.x << " " << source.y << "\n";
			}
		}

		return true;
	}
	return false;
}

