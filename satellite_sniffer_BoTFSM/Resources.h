#pragma once
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include "Sprite.h"

template<typename T>
using sptr = std::shared_ptr<T>;
struct sdl_deleter {
	void operator()(SDL_Window* p) const { SDL_DestroyWindow(p); }

	void operator()(SDL_Renderer* p) const { SDL_DestroyRenderer(p); }

	void operator()(SDL_Texture* p) const { SDL_DestroyTexture(p); }

	void operator()(TTF_Font* p) const { TTF_CloseFont(p); }
};
class Resources {
public:
	static sptr<Resources>& getInstance();
	static void releaseResources();
	TTF_Font* getFont();
	sptr<Sprite>& getMap();
	void clearMap();
	SDL_Rect getMapDimensions();
	sptr<Sprite>& getSat(std::string& type);
	sptr<Sprite>& getButton(const std::string& name);
	sptr<Sprite>& getPoint();

	SDL_Renderer* getRenderer();
	void resetRenderer();
private:
	static sptr<Resources> instance;

	std::unique_ptr<SDL_Window, sdl_deleter> window;
	std::unique_ptr<SDL_Renderer, sdl_deleter> renderer;
	std::unique_ptr<TTF_Font, sdl_deleter> ttffont;

	sptr<Sprite> map;
	sptr<Sprite> cleanMap;
	sptr<Sprite> point;
	std::map<std::string, sptr<Sprite>> buttons;
	std::map<std::string, sptr<Sprite>> sats;

	void release();

	Resources();
	void loadTextures();
};
