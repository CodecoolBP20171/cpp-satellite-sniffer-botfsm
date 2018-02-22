#include "stdafx.h"
#include "Sprite.h"
#include "Resources.h"
#include <SDL_image.h>

Sprite::Sprite(const std::string& fileName) {
	texture = IMG_LoadTexture_RW(Resources::getInstance()->getRenderer(),
		SDL_RWFromFile(fileName.c_str(), "rb"), 1);
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}

Sprite::Sprite(const std::shared_ptr<Sprite>& other) {
	texture = SDL_CreateTexture(Resources::getInstance()->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, other->width, other->height);
	SDL_SetRenderTarget(Resources::getInstance()->getRenderer(), texture);
	other->render(nullptr);
	Resources::getInstance()->resetRenderer();
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}

void Sprite::setAsRenderTarget() {
	SDL_SetRenderTarget(Resources::getInstance()->getRenderer(), texture);
}

Sprite::~Sprite()
{
}
