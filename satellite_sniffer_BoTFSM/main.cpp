// satellite_sniffer_BoTFSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include <iostream>

#include "Satellite.h"
#include "Resources.h"
#include "Texture.h"
#include "LoadSatellites.h"

namespace {
	bool handleEvents() {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return true;
			}
		}
		return false;
	}
}


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	// init resources
	Resources::getInstance();
	std::list<Satellite> sats;
	LoadSatellites::load(sats);
	// main loop

	bool quit(false);
	unsigned int lastTime{0}; // prep for timed calculation
	Uint32 timePassed(0);
	Uint32 timestep(16);

	while (!quit) {
		timePassed = SDL_GetTicks();

		quit = handleEvents();

		SDL_RenderClear(Resources::getInstance()->getRenderer());

		Resources::getInstance()->clearMap();
		auto map(Resources::getInstance()->getMap());
		map->setAsRenderTarget();
		auto mapSize(map->getDimensions());
		if (timePassed > lastTime + 1000) {
			for (auto& sat : sats) {
				sat.setPosition();
			}
			lastTime = timePassed;
		}
		for (auto& sat : sats) {
			sat.render(mapSize);
		}

		Resources::getInstance()->resetRenderer();
		SDL_Rect pos = { 0,0,1280,640 };
		map->render(&pos);

		SDL_RenderPresent(Resources::getInstance()->getRenderer());

		while (timePassed + timestep > SDL_GetTicks()) {
			SDL_Delay(0);
		}

	}

	Resources::releaseResources();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
