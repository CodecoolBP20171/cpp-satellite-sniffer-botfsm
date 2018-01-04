#include "stdafx.h"
#include "UISatellite.h"
#include "Resources.h"
#include "Globals.h"

UISatellite::UISatellite(Satellite& sat) 
	: UIElement({0,0,0,0}, PState::MAIN_SCREEN),
	sat(sat),
	texture(Resources::getInstance()->getSat(sat.getType())),
	trajectoryForward(Resources::getInstance()->getPoint()),
	trajectoryBackward(Resources::getInstance()->getPoint()),
	text(new ScreenText(sat.getName())),
	popupText(new ScreenText(sat.getName()))
{
	text->setColor({ 0,0,0 });
}

void UISatellite::render()
{
	if (!sat.isShown()) return;
	renderTrajectory();
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto satSize(texture->getDimensions());
	auto satpos(sat.getPosition());
	SDL_Rect satRect = {
		static_cast<int>(round(satpos.longitude / (MathConstants::PI * 2) * mapSize.w - satSize.w / 2)),
		static_cast<int>(round(satpos.latitude / (MathConstants::PI) * mapSize.h - satSize.h / 2)),
		satSize.w,
		satSize.h
	};
	texture->render(&satRect);
	auto textPos(text->getDimensions());
	textPos.x = satRect.x + satSize.w;
	textPos.y = satRect.y;
	text->render(&textPos);
}

void UISatellite::popupRender(int pos)
{
	if (sat.isShown()) {
		popupText->setColor({ 0, 200, 0 });
	}
	else {
		popupText->setColor({ 100, 100, 100 });
	}
	auto rect(popupText->getDimensions());
	rect.x = Dimensions::POPUP_OFFSET_X + Dimensions::MENU_BUTTON_SPACING;
	rect.y = Dimensions::POPUP_OFFSET_Y + rect.h * (pos - 1) + Dimensions::MENU_BUTTON_SPACING * pos;
	popupText->render(&rect);
}

void UISatellite::renderTrajectory()
{
	std::time_t forward;
	std::time(&forward);
	auto back(forward);
	auto delta(sat.getDelta());

	for (int i = 0; i < 8; ++i) {
		forward += delta;
		back -= delta;
		renderPoint(forward, trajectoryForward);
		renderPoint(back, trajectoryBackward);
	}
}

void UISatellite::renderPoint(std::time_t & now, std::shared_ptr<Sprite>& point)
{
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto pos(sat.getPositionAtTime(now));
	auto pointSize(point->getDimensions());

	SDL_Rect pointRect = {
		static_cast<int>(round(pos.longitude / (MathConstants::PI * 2) * mapSize.w - pointSize.w / 2)),
		static_cast<int>(round(pos.latitude / (MathConstants::PI) * mapSize.h - pointSize.h / 2)),
		pointSize.w,
		pointSize.h
	};
	point->render(&pointRect);
}

