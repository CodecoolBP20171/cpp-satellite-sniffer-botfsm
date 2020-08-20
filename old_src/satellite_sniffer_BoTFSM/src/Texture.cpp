#include "stdafx.h"
#include "Texture.h"
#include "Resources.h"

#include <GL/glew.h>

Texture::~Texture() {
    if (texture) SDL_DestroyTexture(texture);
}

void Texture::render(const SDL_Rect* pos) {
    SDL_RenderCopy(Resources::getInstance()->getRenderer(), texture, nullptr, pos);
}

void Texture::render(const SDL_Rect* source, const SDL_Rect* dest)
{
    SDL_RenderCopy(Resources::getInstance()->getRenderer(), texture, source, dest);
}

SDL_Rect Texture::getDimensions() {
    return { 0, 0, width, height };
}
