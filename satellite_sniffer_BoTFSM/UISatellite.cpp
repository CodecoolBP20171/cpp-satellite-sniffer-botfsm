#include "stdafx.h"
#include "UISatellite.h"
#include "Resources.h"
#include "Globals.h"
#include "Config.h"
#include <list>
#include <SDL2_gfxPrimitives.h>

int UISatellite::pos{ 0 };

UISatellite::UISatellite(Satellite& sat)
	: UIElement({ 0,0,0,0 }, PState::MAIN_SCREEN),
	sat(sat),
	texture(Resources::getInstance()->getSat(sat.getType())),
	trajectoryForward(sat.getForwardTrajectory()),
	trajectoryBackward(sat.getBackTrajectory()),
	text(new ScreenText(sat.getName())),
	popupText(new ScreenText(sat.getName()))
{
	rect = popupText->getDimensions();
	double oldH(rect.h);
	rect.h = Config::getIntOption("Dimensions", "POPUP_HEIGHT") / 10;
	rect.w = static_cast<int>(rect.w * (rect.h / oldH));
	rect.x = Config::getIntOption("Dimensions", "POPUP_OFFSET_X") + Config::getIntOption("Dimensions", "MENU_BUTTON_SPACING");
	if (pos >= 10) {
		rect.x += Config::getIntOption("Dimensions", "POPUP_WIDTH") / 2;
	}
	rect.y = Config::getIntOption("Dimensions", "POPUP_OFFSET_Y") + rect.h * (pos % 10);
	text->setColor({ 0,0,0 });
	++pos;
}

void UISatellite::render(int zoom)
{
	if (!sat.isShown()) return;
	renderTrajectory();
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto satSize(texture->getDimensions());
	auto satpos(sat.getPosition());
	zoom = std::pow(2, zoom);
	SDL_Rect satRect = {
		static_cast<int>(round(satpos.longitude / (MathConstants::PI * 2) * mapSize.w - satSize.w / (2 * zoom))),
		static_cast<int>(round(satpos.latitude / (MathConstants::PI) * mapSize.h - satSize.h / (2 * zoom))),
		satSize.w / zoom,
		satSize.h / zoom
	};
	texture->render(&satRect);
	auto textPos(text->getDimensions());
	textPos.x = satRect.x + satSize.w;
	textPos.y = satRect.y;
	text->render(&textPos);
}

bool UISatellite::isClicked(const SDL_MouseButtonEvent e, PState & state)
{
	if (e.button == SDL_BUTTON_LEFT && UIElement::isClicked(e, state)) {
		sat.toggleShown();
		return true;
	}
	return false;
}

void UISatellite::popupRender()
{
	if (sat.isShown()) {
		popupText->setColor({ 15, 200, 15 });
	}
	else {
		popupText->setColor({ 100, 100, 100 });
	}
	popupText->render(&rect);
}


void UISatellite::renderTrajectory()
{
	trajectoryForward.render();
	trajectoryBackward.render();
}
