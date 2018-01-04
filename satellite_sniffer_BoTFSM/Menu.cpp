#include "stdafx.h"
#include "Menu.h"
#include "Globals.h"


Menu::Menu(SDL_Rect rect, PState state) : UIElement(rect, state)
{
	rect.h = Dimensions::MENU_BUTTON_HEIGHT;
	rect.w = Dimensions::MENU_BUTTON_WIDTH;
	rect.x += Dimensions::MENU_BUTTON_SPACING;
	rect.y += Dimensions::MENU_BUTTON_VERTICAL_SPACING;
	menuButtons.emplace_back(new Button(rect, state, ButtonName::SATELLITES));
}

bool Menu::isClicked(const int x, const int y, PState & state)
{
	if (UIElement::isClicked(x, y, state)) {
		for (auto& button : menuButtons) {
			if (button->isClicked(x, y, state)) {
				break;
			}
		}
		return true;
	}
	return false;
}

void Menu::render()
{
	for (auto& button : menuButtons) {
		button->render();
	}
}

