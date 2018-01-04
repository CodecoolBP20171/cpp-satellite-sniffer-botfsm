#include "stdafx.h"
#include "Popup.h"
#include "Globals.h"


Popup::Popup(SDL_Rect rect, PState state) : UIElement(rect, state)
{
	SDL_Rect buttonRect(rect);
	buttonRect.h = Dimensions::MENU_BUTTON_HEIGHT / 2;
	buttonRect.w = Dimensions::MENU_BUTTON_WIDTH / 2;
	buttonRect.x += Dimensions::POPUP_WIDTH - buttonRect.w - Dimensions::MENU_BUTTON_SPACING;
	buttonRect.y += Dimensions::POPUP_HEIGHT - buttonRect.h - Dimensions::MENU_BUTTON_SPACING;
	button.reset(new Button(buttonRect, state, ButtonName::OK));
}

bool Popup::isClicked(const int x, const int y, PState & state)
{
	return false;
}

void Popup::render()
{

}

