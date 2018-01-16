#include "stdafx.h"
#include "Globals.h"
#include "Program.h"
#include "Popup.h"
#include "Resources.h"
#include "SatelliteLoader.h"
#include "Menu.h"
#include "Map.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

Program::Program() :
	quit(false),
	loaded(false),
	timePassed(0),
	timestep(16), // frame time length 1000 / 60
	lastCalculationTime(0),
	calculationTimeStep(1000), // 1 sec
	state(PState::MAIN_SCREEN)
{}


Program::~Program()
{
	if (loaded) unload();
}

void Program::init()
{
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	// init resources
	Resources::getInstance();
	SatelliteLoader::loadSatellites(sats);
	for (auto& sat : sats) {
		UISats.emplace_back(sat);
	}

	UIElements.emplace_back(new Map(UIRects::MAP, PState::MAIN_SCREEN, UISats));
	UIElements.emplace_back(new Menu(UIRects::MENU, PState::MAIN_SCREEN));
	UIElements.emplace_back(new Popup(UIRects::POPUP, PState::MENU_SCREEN, UISats));
	loaded = true;
}

void Program::run()
{
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

void Program::unload()
{
	Resources::releaseResources();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	loaded = false;
}

void Program::updatePositions()
{
	if (timePassed > lastCalculationTime + 1000) {
		for (auto& sat : sats) {
			sat.updatePosition();
		}
		lastCalculationTime = timePassed;
	}
}

void Program::render()
{
	SDL_SetRenderDrawColor(Resources::getInstance()->getRenderer(), 50, 50, 50, 255);
	for (auto& elem : UIElements) {
		if (elem->isActive(state)) {
			elem->render();
		}
	}
	SDL_RenderPresent(Resources::getInstance()->getRenderer());
}


bool Program::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return true;
		}
		if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
			for (auto& elem : UIElements) {
				if (elem->isActive(state) && elem->isClicked(e.button.x, e.button.y, state)) {
					break;
				}
			}
		}
	}
	return false;
}
