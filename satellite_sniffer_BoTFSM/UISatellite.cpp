#include "stdafx.h"
#include "UISatellite.h"
#include "Resources.h"
#include "Globals.h"
#include <list>
#include <SDL2_gfxPrimitives.h>

int UISatellite::pos{ 1 };

UISatellite::UISatellite(Satellite& sat)
	: UIElement({ 0,0,0,0 }, PState::MAIN_SCREEN),
	sat(sat),
	texture(Resources::getInstance()->getSat(sat.getType())),
	trajectoryForward(Resources::getInstance()->getPoint()),
	trajectoryBackward(Resources::getInstance()->getPoint()),
	text(new ScreenText(sat.getName())),
	popupText(new ScreenText(sat.getName()))
{
	rect = popupText->getDimensions();
	double oldH(rect.h);
	rect.h = Dimensions::POPUP_HEIGHT / 10;
	rect.w *= rect.h / oldH;
	rect.x = Dimensions::POPUP_OFFSET_X + Dimensions::MENU_BUTTON_SPACING;
	if (pos > 10) {
		rect.x += Dimensions::POPUP_WIDTH / 2;
	}
	rect.y = Dimensions::POPUP_OFFSET_Y + rect.h * (pos % 10);
	text->setColor({ 0,0,0 });
	++pos;
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

bool UISatellite::isClicked(const int x, const int y, PState & state)
{
	if (UIElement::isClicked(x, y, state)) {
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
	std::list<CoordGeodetic> trajectoryPoints;
	std::time_t forward;
	std::time(&forward);
	auto back(forward);
	auto forwarddelta(sat.getDelta(forward));
	auto backdelta(sat.getDelta(back));
	double threshold(0.5);
	int maxpoints(100);
	int cntr(0);

	while (trajectoryPoints.size() <= 1
		   || (getDistance(trajectoryPoints.front(), trajectoryPoints.back()) < threshold
		   && cntr < maxpoints)) {
		forward += forwarddelta;
		forwarddelta = sat.getDelta(forward);
		back -= backdelta;
		backdelta = sat.getDelta(back);
		trajectoryPoints.push_front(sat.getPositionAtTime(back));
		trajectoryPoints.push_back(sat.getPositionAtTime(forward));
		cntr += 2;
	}

	for (auto& point : trajectoryPoints) {
		renderPoint(point, trajectoryForward);
	}

	/*for (int i = 0; i < 8; ++i) {
		forward += delta;
		back -= delta;
		renderPoint(forward, trajectoryForward);
		renderPoint(back, trajectoryBackward);
	}*/
}

void UISatellite::renderPoint(const CoordGeodetic& coord, std::shared_ptr<Sprite>& point)
{
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto pointSize(10);

	SDL_Rect pointRect = {
		static_cast<int>(round(coord.longitude / (MathConstants::PI * 2) * mapSize.w - pointSize / 2)),
		static_cast<int>(round(coord.latitude / (MathConstants::PI) * mapSize.h - pointSize / 2)),
		pointSize,
		pointSize
	};
	filledCircleRGBA(Resources::getInstance()->getRenderer(),
					 pointRect.x, pointRect.y, pointSize, 255, 0, 0, 255);
	//point->render(&pointRect);
}

double UISatellite::getDistance(const CoordGeodetic & a, const CoordGeodetic & b)
{
	return sqrt(pow(a.latitude - b.latitude, 2) + pow(a.longitude - b.longitude, 2));
}

