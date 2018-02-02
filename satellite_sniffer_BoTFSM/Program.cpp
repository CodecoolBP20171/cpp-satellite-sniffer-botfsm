#include "stdafx.h"
#include "Config.h"
#include "Program.h"
#include "Popup.h"
#include "Resources.h"
#include "Satellites.h"
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
	calculationTimeStep(5000), // 1 sec
	zoom(Config::getIntOption("ZoomLevel", "MIN")),
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
	Satellites::getInstance();

	UIElements.emplace_back(new Map(Config::getRect("MAP"), PState::MAIN_SCREEN, zoom));
	UIElements.emplace_back(new Menu(Config::getRect("MENU"), PState::MAIN_SCREEN));
	UIElements.emplace_back(new Popup(Config::getRect("POPUP"), PState::MENU_SCREEN));
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
	if (timePassed > lastCalculationTime + calculationTimeStep) {
		Satellites::getInstance()->updatePosition();
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
	int x, y;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return true;
		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			for (auto& elem : UIElements) {
				if (elem->isActive(state) && elem->isClicked(e.button, state)) {
					break;
				}
			}
		}
		if (PState::QUIT == state) {
			return true;
		}
	}
	return false;
}
