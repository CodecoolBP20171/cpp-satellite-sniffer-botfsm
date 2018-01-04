#include "stdafx.h"
#include "ScreenText.h"
#include "Resources.h"
#include <iostream>

ScreenText::ScreenText(std::string& text) {
	SDL_Color color = { 255, 255, 255 };
	auto textSurface(TTF_RenderText_Blended(Resources::getInstance()->getFont(), text.c_str(), color));
	texture = SDL_CreateTextureFromSurface(Resources::getInstance()->getRenderer(), textSurface);
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	SDL_FreeSurface(textSurface);
}

void ScreenText::setAsRenderTarget() {
	std::cerr << "Illegal operation: text cannot be set as render target." << std::endl;
}

void ScreenText::setColor(SDL_Color color)
{
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}
