#include "stdafx.h"
#include "ScreenText.h"
#include "Resources.h"


ScreenText::ScreenText(std::string& text) {
	SDL_Color color = { 0,0,0 };
	auto textSurface(TTF_RenderText_Blended(Resources::getInstance()->getFont(), text.c_str(), color));
	texture = SDL_CreateTextureFromSurface(Resources::getInstance()->getRenderer(), textSurface);
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	SDL_FreeSurface(textSurface);
}


ScreenText::~ScreenText() {
	if (texture) SDL_DestroyTexture(texture);
}

void ScreenText::render(const SDL_Rect * pos) {
	SDL_RenderCopy(Resources::getInstance()->getRenderer(), texture, nullptr, pos);
}

SDL_Rect ScreenText::getDimensions() {
	return { 0, 0, width, height };
}
