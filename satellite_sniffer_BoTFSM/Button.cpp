#include "stdafx.h"
#include "Button.h"
#include "Resources.h"
#include "Config.h"

Button::Button(SDL_Rect rect, PState state, const std::string& name)
	: UIElement(rect, state),
	name(name),
	sprite(Resources::getInstance()->getButton(name))
{}

bool Button::isClicked(const int x, const int y, PState & state)
{
	if (UIElement::isClicked(x, y, state)) {
		if (name == Config::getStringOption("ButtonName", "SATELLITES")) {
			state = PState::PAUSED;
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
