#include "stdafx.h"
#include "Globals.h"
#include "Program.h"
#include "Resources.h"
#include "SatelliteLoader.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

Program::Program()
	:quit(false),
	loaded(false),
	timePassed(0),
	timestep(16), // frame time length 1000 / 60
	lastCalculationTime(0),
	calculationTimeStep(1000) // 1 sec
{}


Program::~Program() {
	if (loaded) unload();
}

void Program::init() {
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	// init resources
	Resources::getInstance();
	SatelliteLoader::loadSatellites(sats);
	loaded = true;
}

void Program::run() {
	if (!loaded) init();
	while (!quit) {
		timePassed = SDL_GetTicks();
		quit = handleEvents();

		updatePositions();
		render();

		/* wait for next frame */
		while (timePassed + timestep > SDL_GetTicks()) {
			SDL_Delay(0);
		}

	}
}

void Program::unload() {
	Resources::releaseResources();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	loaded = false;
}

void Program::updatePositions() {
	if (timePassed > lastCalculationTime + 1000) {
		for (auto& sat : sats) {
			sat.updatePosition();
		}
		lastCalculationTime = timePassed;
	}
}

void Program::render() {
	SDL_RenderClear(Resources::getInstance()->getRenderer());
	Resources::getInstance()->clearMap();
	Resources::getInstance()->getMap()->setAsRenderTarget();
	for (auto& sat : sats) {
		sat.render();
	}

	Resources::getInstance()->resetRenderer();
	SDL_Rect pos = {
		0,
		Dimensions::MENU_HEIGHT,
		Dimensions::MAP_WIDTH,
		Dimensions::MAP_HEIGHT
	};
	Resources::getInstance()->getMap()->render(&pos);

	SDL_RenderPresent(Resources::getInstance()->getRenderer());
}

bool Program::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return true;
		}
	}
	return false;
}
