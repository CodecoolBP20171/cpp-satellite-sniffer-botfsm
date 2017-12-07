#include "stdafx.h"
#include "Resources.h"
#include <iostream>


sptr<Resources> Resources::instance = nullptr;
Resources::Resources() {
	std::unique_ptr<SDL_Window, sdl_deleter> win(SDL_CreateWindow("Satellites",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		960, 640, SDL_WINDOW_SHOWN));
	if (!win) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	}
	win.swap(window);

	std::unique_ptr<SDL_Renderer, sdl_deleter> ren(SDL_CreateRenderer(window.get(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!ren) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
	}
	ren.swap(renderer);

	std::unique_ptr<TTF_Font, sdl_deleter> fon(TTF_OpenFont("Mermaid1001.ttf", 128));
	if (!fon) {
		std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
	}
	fon.swap(ttffont);
}

sptr<Resources>& Resources::getInstance() {
	if (!instance) {
		instance = sptr<Resources>(new Resources());
		instance->loadTextures();
	}
	return instance;
}

void Resources::loadTextures() {
	cleanMap = sptr<Texture>(new Texture("map.png"));
	map = sptr<Texture>(new Texture(cleanMap));

	sats.emplace("STATION", sptr<Texture>(new Texture("station.png")));
	sats.emplace("TELESCOPE", sptr<Texture>(new Texture("telescope.png")));
	sats.emplace("GPS", sptr<Texture>(new Texture("gps.png")));
}


SDL_Renderer* Resources::getRenderer() {
	return renderer.get();
}

void Resources::resetRenderer() {
	SDL_SetRenderTarget(renderer.get(), nullptr);
}


void Resources::releaseResources() {
	if (instance) instance->release();
}

void  Resources::renderText(const std::string& text, SDL_Rect position)
{
	SDL_Color color = { 0,0,0 };
	auto textSurface(TTF_RenderUTF8_Blended(ttffont.get(), text.c_str(), color));
	auto finalText(SDL_CreateTextureFromSurface(renderer.get(), textSurface));
	SDL_QueryTexture(finalText, nullptr, nullptr, &position.w, &position.h);
	SDL_RenderCopy(renderer.get(), finalText, nullptr, &position);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(finalText);
}

sptr<Texture>& Resources::getMap() {
	return map;
}

void Resources::clearMap() {
	map->setAsRenderTarget();
	cleanMap->render(nullptr);
	resetRenderer();
}

sptr<Texture>& Resources::getSat(std::string& type) {
	return sats[type];
}

void Resources::release() {
	ttffont.reset();
	map.reset();
	cleanMap.reset();
	for (auto& sat : sats) {
		sat.second.reset();
	}
	renderer.reset();
	window.reset();
}