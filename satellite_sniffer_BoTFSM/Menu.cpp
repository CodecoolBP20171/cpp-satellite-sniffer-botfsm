#include "stdafx.h"
#include "Menu.h"
#include "Globals.h"


Menu::Menu(SDL_Rect rect) : UIElement(rect)
{
	rect.h = Dimensions::MENU_BUTTON_HEIGHT;
	rect.w = Dimensions::MENU_BUTTON_WIDTH;
	rect.x += Dimensions::MENU_BUTTON_SPACING;
	rect.y += Dimensions::MENU_BUTTON_VERTICAL_SPACING;
	menuButtons.emplace_back(new Button(rect, ButtonName::SATELITES));
}

void Menu::render()
{
	for (auto& button : menuButtons) {
		button->render();
	}
}

