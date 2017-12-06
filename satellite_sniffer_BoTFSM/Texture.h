#pragma once
#include <string>
#include <SDL_image.h>
class Texture {
public:
	explicit Texture(const std::string fileName);
	virtual ~Texture();
	void render(const SDL_Rect* pos);
	SDL_Rect getDimensions();
private:
	SDL_Texture* texture;
	int width, height;
};

