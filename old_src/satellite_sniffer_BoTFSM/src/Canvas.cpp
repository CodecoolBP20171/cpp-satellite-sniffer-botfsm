#include "stdafx.h"
#include "Canvas.h"
#include "Resources.h"
#include <SDL.h>

Canvas::Canvas(const int width, const int height)
{
	this->height = height;
	this->width = width;
	texture = SDL_CreateTexture(Resources::getInstance()->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Canvas::resize(const int width, const int height)
{
	if (texture) SDL_DestroyTexture(texture);
	this->height = height;
	this->width = width;
	texture = SDL_CreateTexture(Resources::getInstance()->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void Canvas::setAsRenderTarget()
{
	SDL_SetRenderTarget(Resources::getInstance()->getRenderer(), texture);
}


Canvas::~Canvas()
{
}
