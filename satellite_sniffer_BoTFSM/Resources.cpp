#include "stdafx.h"
#include "Globals.h"
#include "Resources.h"
#include "LoadError.hpp"
#include <iostream>


sptr<Resources> Resources::instance = nullptr;
Resources::Resources() {
	std::unique_ptr<SDL_Window, sdl_deleter> win(SDL_CreateWindow("Satellites",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		Dimensions::WINDOW_WIDTH,
		Dimensions::WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN));
	if (!win) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		throw LoadError();
	}
	win.swap(window);

	std::unique_ptr<SDL_Renderer, sdl_deleter> ren(SDL_CreateRenderer(window.get(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!ren) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		throw LoadError();
	}
	ren.swap(renderer);

	std::unique_ptr<TTF_Font, sdl_deleter> fon(TTF_OpenFont(FontFiles::MAP_TEXT.c_str(), Dimensions::MAP_TEXT_SIZE));
	if (!fon) {
		std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		throw LoadError();
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
	cleanMap = sptr<Sprite>(new Sprite(TextureFiles::MAP));
	map = sptr<Sprite>(new Sprite(cleanMap));
	point = sptr<Sprite>(new Sprite(TextureFiles::PATH));

	buttons.emplace(ButtonName::SATELLITES, sptr<Sprite>(new Sprite(TextureFiles::SAT_BUTTON)));
	buttons.emplace(ButtonName::OK, sptr<Sprite>(new Sprite(TextureFiles::OK_BUTTON)));

	sats.emplace(SatelliteType::STATION, sptr<Sprite>(new Sprite(TextureFiles::STATION)));
	sats.emplace(SatelliteType::TELESCOPE, sptr<Sprite>(new Sprite(TextureFiles::TELESCOPE)));
	sats.emplace(SatelliteType::GPS, sptr<Sprite>(new Sprite(TextureFiles::GPS)));
}


SDL_Renderer* Resources::getRenderer() {
	return renderer.get();
}

void Resources::resetRenderer() {
	SDL_SetRenderTarget(renderer.get(), nullptr);
}


void Resources::releaseResources() {
	if (instance) instance->release();
	instance.reset();
}

TTF_Font * Resources::getFont() {
	return ttffont.get();
}


sptr<Sprite>& Resources::getMap() {
	return map;
}

sptr<Sprite>& Resources::getCleanMap() {
	return cleanMap;
}

void Resources::clearMap() {
	map->setAsRenderTarget();
	cleanMap->render(nullptr);
	resetRenderer();
}

SDL_Rect Resources::getMapDimensions() {
	return map->getDimensions();
}

sptr<Sprite>& Resources::getSat(std::string& type) {
	return sats[type];
}

sptr<Sprite>& Resources::getButton(const std::string & name)
{
	return buttons[name];
}

sptr<Sprite>& Resources::getPoint() {
	return point;
}

void Resources::release() {
	ttffont.reset();
	map.reset();
	cleanMap.reset();
	point.reset();
	for (auto& sat : sats) {
		sat.second.reset();
	}
	for (auto& button : buttons) {
		button.second.reset();
	}
	renderer.reset();
	window.reset();
}