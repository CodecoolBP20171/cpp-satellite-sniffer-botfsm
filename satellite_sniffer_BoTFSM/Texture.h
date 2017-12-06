#pragma once
#include <string>
#include <SDL_image.h>
#include <memory>
class Texture {
public:
	explicit Texture(const std::string fileName);
	explicit Texture(const std::shared_ptr<Texture>& other);
	virtual ~Texture();
	void render(const SDL_Rect* pos);
	SDL_Texture* get();
	SDL_Rect getDimensions();
private:
	SDL_Texture* texture;
	int width, height;
};

