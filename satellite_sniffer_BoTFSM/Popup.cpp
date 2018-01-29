#include "stdafx.h"
#include "Popup.h"
#include "Resources.h"
#include "SatelliteLoader.h"
#include "Config.h"

Popup::Popup(SDL_Rect rect, PState state, std::list<UISatellite>& satList) : 
	UIElement(rect, state),
	sats(satList)
{
	SDL_Rect buttonRect(rect);
	buttonRect.h = Config::getIntOption("Dimensions", "MENU_BUTTON_HEIGHT") / 2;
	buttonRect.w = Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH") / 2;
	buttonRect.x += Config::getIntOption("Dimensions", "POPUP_WIDTH") - buttonRect.w - Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	buttonRect.y += Config::getIntOption("Dimensions", "POPUP_HEIGHT") - buttonRect.h - Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	button.reset(new Button(buttonRect, state, Config::getStringOption("ButtonName", "OK")));
}

bool Popup::isClicked(const int x, const int y, PState & state)
{
	if (UIElement::isClicked(x, y, state)) {
		if (button->isClicked(x, y, state)) {
			SatelliteLoader::saveSatelliteList(sats);
			state = PState::MAIN_SCREEN;
		} else {
			for (auto& sat : sats) {
				if (sat.isClicked(x, y, state)) {
					break;
				}
			}
		}
		return true;
	}
	return false;
}

void Popup::render()
{
	SDL_SetRenderDrawColor(Resources::getInstance()->getRenderer(), 50, 50, 50, 255);
	SDL_RenderFillRect(Resources::getInstance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(Resources::getInstance()->getRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(Resources::getInstance()->getRenderer(), &rect);
	for (auto& sat : sats) {
		sat.popupRender();
	}
	button->render();
}

