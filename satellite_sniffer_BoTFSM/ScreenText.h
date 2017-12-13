#pragma once
#include <string>
#include <SDL.h>

class ScreenText {
public:
	ScreenText(std::string& text);
	virtual ~ScreenText();
	void render(const SDL_Rect* pos);
	SDL_Rect getDimensions();
private:
	SDL_Texture * texture;
	int width, height;
};

