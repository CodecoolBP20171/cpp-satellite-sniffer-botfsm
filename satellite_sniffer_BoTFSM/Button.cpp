#include "stdafx.h"
#include "Button.h"
#include "Resources.h"

Button::Button(SDL_Rect rect, const std::string& name) 
	: UIElement(rect), 
	sprite(Resources::getInstance()->getButton(name))
{
}

void Button::render()
{
	sprite->render(&rect);
}
