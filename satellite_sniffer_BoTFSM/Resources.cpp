#include "stdafx.h"
#include "Resources.h"
#include <iostream>


sptr<Resources> Resources::instance = nullptr;
Resources::Resources() :
	renderer(SDL_CreateRenderer(window.get(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
	std::unique_ptr<SDL_Window, sdl_deleter> win(SDL_CreateWindow("TheMadSquirrels : Stratego",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		960, 640, SDL_WINDOW_SHOWN));
	if (!win) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	}
	win.swap(window);
}

sptr<Resources>& Resources::getInstance() {
	if (!instance) {
		instance = sptr<Resources>(new Resources());
		instance->loadTextures();
	}
	return instance;
}

void Resources::loadTextures() {
	
}


SDL_Renderer* Resources::getRenderer() {
	return renderer.get();
}


void Resources::releaseResources() {
	if (instance) instance->release();
}

void Resources::release() {
	
	renderer.reset();
	window.reset();
}