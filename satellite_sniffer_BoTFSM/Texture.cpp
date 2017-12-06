#include "stdafx.h"
#include "Texture.h"
#include "Resources.h"


Texture::Texture(const std::string fileName)
	: texture(IMG_LoadTexture_RW(Resources::getInstance()->getRenderer(),
		SDL_RWFromFile(fileName.c_str(), "rb"),
		1)) {
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}

Texture::Texture(const std::shared_ptr<Texture>& other) {
	texture = SDL_CreateTexture(Resources::getInstance()->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, other->width, other->height);
	SDL_SetRenderTarget(Resources::getInstance()->getRenderer(), texture);
	other->render(nullptr);
	Resources::getInstance()->resetRenderer();
}

Texture::~Texture() {
	if (texture) SDL_DestroyTexture(texture);
}

void Texture::render(const SDL_Rect* pos) {
	SDL_RenderCopy(Resources::getInstance()->getRenderer(), texture, nullptr, pos);
}

SDL_Texture * Texture::get() {
	return texture;
}

SDL_Rect Texture::getDimensions() {
	return { 0, 0, width, height };
}
