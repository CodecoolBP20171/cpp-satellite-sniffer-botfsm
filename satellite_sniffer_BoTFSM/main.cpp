// satellite_sniffer_BoTFSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <iostream>

#include "Satellite.h"
#include "Resources.h"
#include "MechStandards.h"
#include "Texture.h"

namespace {
	void loadSatellites(std::list<Satellite> &sats) {
		std::ifstream file("satellites/satellites.dat");
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream sat(line);
			std::string name, noradId, type;
			std::getline(sat, name, ';');
			std::getline(sat, noradId, ';');
			std::getline(sat, type, ';');
			sats.emplace_back(name, noradId, type);
		}
		file.close();
	}

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
	loadSatellites(sats);
	// main loop

	bool quit(false);

	Uint32 timePassed(0);
	Uint32 timestep(16);

	while (!quit) {
		timePassed = SDL_GetTicks();

		quit = handleEvents();

		SDL_RenderClear(Resources::getInstance()->getRenderer());

		Texture map(Resources::getInstance()->getMap());
		map.setAsRenderTarget();
		auto mapSize(map.getDimensions());
		
		for (auto& sat : sats) {
			sat.render(mapSize);
		}

		Resources::getInstance()->resetRenderer();
		SDL_Rect pos = { 0,160,960,480 };
		map.render(&pos);

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
