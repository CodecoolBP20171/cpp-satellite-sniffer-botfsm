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
}

sptr<Resources>& Resources::getInstance() {
	if (!instance) {
		instance = sptr<Resources>(new Resources());
		instance->loadTextures();
	}
	return instance;
}

void Resources::loadTextures() {
	map = sptr<Texture>(new Texture("map.png"));
	sat = sptr<Texture>(new Texture("sat.png"));
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

Texture Resources::getMap() {
	return Texture(map);
}

sptr<Texture>& Resources::getSat() {
	return sat;
}

void Resources::release() {
	map.reset();
	sat.reset();
	renderer.reset();
	window.reset();
}