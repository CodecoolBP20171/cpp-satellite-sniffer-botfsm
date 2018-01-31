#include "stdafx.h"
#include "Popup.h"
#include "Resources.h"
#include "Satellites.h"
#include "Config.h"

Popup::Popup(SDL_Rect rect, PState state) : 
	UIElement(rect, state)
{
	SDL_Rect buttonRect(rect);
	buttonRect.h = Config::getIntOption("Dimensions", "MENU_BUTTON_HEIGHT") / 2;
	buttonRect.w = Config::getIntOption("Dimensions", "MENU_BUTTON_WIDTH") / 2;
	buttonRect.x += Config::getIntOption("Dimensions", "POPUP_WIDTH") - buttonRect.w - Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	buttonRect.y += Config::getIntOption("Dimensions", "POPUP_HEIGHT") - buttonRect.h - Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	button.reset(new Button(buttonRect, state, Config::getStringOption("ButtonName", "OK")));
}

bool Popup::isClicked(const SDL_MouseButtonEvent e, PState & state)
{
	if (e.button == SDL_BUTTON_LEFT && UIElement::isClicked(e, state)) {
		if (button->isClicked(e, state)) {
			Satellites::getInstance()->saveSatelliteList();
			state = PState::MAIN_SCREEN;
		} else {
			for (auto& sat : Satellites::getInstance()->getUISatellites()) {
				if (sat.isClicked(e, state)) {
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
	Satellites::getInstance()->renderPopupSatellits();
	button->render();
}

