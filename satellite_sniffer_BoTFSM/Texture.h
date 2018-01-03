#pragma once
#include <SDL.h>
class Texture {
public:
	virtual ~Texture();
	void render(const SDL_Rect* pos);
	SDL_Rect getDimensions();
	virtual void setAsRenderTarget() = 0;
protected:
	SDL_Texture * texture;
	int width, height;
};
