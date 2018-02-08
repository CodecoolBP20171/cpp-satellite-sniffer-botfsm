#include "stdafx.h"
#include "Button.h"
#include "Resources.h"
#include "Config.h"

Button::Button(SDL_Rect rect, PState state, const std::string& name)
	: UIElement(rect, state),
	name(name),
	sprite(Resources::getInstance()->getButton(name))
{}

bool Button::isClicked(const SDL_MouseButtonEvent e, PState & state)
{
	if (e.button == SDL_BUTTON_LEFT && UIElement::isClicked(e, state)) {
		if (name == Config::getStringOption("ButtonName", "SATELLITES")) {
			state = PState::MENU_SCREEN;
		}
		else if (name == Config::getStringOption("ButtonName", "EXIT")) {
			state = PState::QUIT;
		}
		return true;
	}
	return false;
}

void Button::render()
{
	sprite->render(&rect);
}
