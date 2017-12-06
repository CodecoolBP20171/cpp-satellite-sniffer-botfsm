#pragma once
#include <memory>
#include <SDL.h>
#include "Texture.h"
template<typename T>
using sptr = std::shared_ptr<T>;
struct sdl_deleter {
	void operator()(SDL_Window* p) const { SDL_DestroyWindow(p); }

	void operator()(SDL_Renderer* p) const { SDL_DestroyRenderer(p); }

	void operator()(SDL_Texture* p) const { SDL_DestroyTexture(p); }
};
class Resources {
public:
	static sptr<Resources>& getInstance();
	static void releaseResources();
	
	Texture getMap();
	sptr<Texture>& getSat();

	SDL_Renderer* getRenderer();
	void resetRenderer();
private:
	static sptr<Resources> instance;

	std::unique_ptr<SDL_Window, sdl_deleter> window;
	std::unique_ptr<SDL_Renderer, sdl_deleter> renderer;

	sptr<Texture> map;
	sptr<Texture> sat;

	void release();

	Resources();
	void loadTextures();
};
