#include "stdafx.h"
#include "Texture.h"
#include "Resources.h"


Texture::Texture(const std::string fileName)
	: texture(IMG_LoadTexture_RW(Resources::getInstance()->getRenderer(),
		SDL_RWFromFile(fileName.c_str(), "rb"),
		1)) {
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}

Texture::~Texture() {
	if (texture) SDL_DestroyTexture(texture);
}

void Texture::render(const SDL_Rect* pos) {
	SDL_RenderCopy(Resources::getInstance()->getRenderer(), texture, nullptr, pos);
}

SDL_Rect Texture::getDimensions() {
	return { 0, 0, width, height };
}
