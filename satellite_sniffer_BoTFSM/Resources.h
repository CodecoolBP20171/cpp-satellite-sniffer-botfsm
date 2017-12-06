#pragma once
#include <memory>
#include <SDL.h>
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

	SDL_Renderer* getRenderer();
private:
	static sptr<Resources> instance;

	std::unique_ptr<SDL_Window, sdl_deleter> window;
	std::unique_ptr<SDL_Renderer, sdl_deleter> renderer;
	void release();

	Resources();
	void loadTextures();
};
