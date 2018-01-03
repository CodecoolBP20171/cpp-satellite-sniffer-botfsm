#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include "Satellite.h"
#include "Resources.h"
#include "Globals.h"
#include "Resources.h"

#include <SGP4.h>
#include <CoordGeodetic.h>

Satellite::Satellite(std::string name, std::string noradId, std::string type)
	: name(name),
	noradId(noradId),
	type(type),
	tle(Satellite::loadTle(name, noradId)),
	sgp4(tle),
	texture(Resources::getInstance()->getSat(type)),
	trajectoryForward(Resources::getInstance()->getPoint()),
	trajectoryBackward(Resources::getInstance()->getPoint()),
	text(new ScreenText(name)),
	_shown(true) {
	updatePosition();
}

Tle Satellite::loadTle(const std::string & name, const std::string & noradId) {
	std::ifstream file("satellites/" + noradId + ".dat");
	std::string tle1, tle2;
	std::getline(file, tle1);
	std::getline(file, tle2);
	file.close();
	return Tle(name, tle1, tle2);
}

void Satellite::calculate(std::tm& time) {
	DateTime tm(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	Eci eci(sgp4.FindPosition(tm));
	CoordGeodetic geo(eci.ToGeodetic());
	satpos.longitude = geo.longitude;
	satpos.latitude = geo.latitude;
}

void Satellite::calculate() {
	DateTime tm(DateTime::Now(true));
	Eci eci(sgp4.FindPosition(tm));
	CoordGeodetic geo(eci.ToGeodetic());
	satpos.longitude = geo.longitude;
	satpos.latitude = geo.latitude;
}

void Satellite::renderTrajectory() {
	std::time_t forward;
	std::time(&forward);
	auto back(forward);
	auto delta(static_cast<int>(round((24 * 60 * 60) / tle.MeanMotion() / 200)));

	for (int i = 0; i < 8; ++i) {
		forward += delta;
		back -= delta;
		renderPoint(forward, trajectoryForward);
		renderPoint(back, trajectoryBackward);
	}
}

void Satellite::renderPoint(std::time_t & now, std::shared_ptr<Sprite>& point) {
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto pos(getPositionAtTime(now));
	auto pointSize(point->getDimensions());

	SDL_Rect pointRect = {
		static_cast<int>(round(pos.longitude / (MathConstants::PI * 2) * mapSize.w - pointSize.w / 2)),
		static_cast<int>(round(pos.latitude / (MathConstants::PI) * mapSize.h - pointSize.h / 2)),
		pointSize.w,
		pointSize.h
	};
	point->render(&pointRect);
}

GeoCoordinate Satellite::getPositionAtTime(std::time_t& time) {
	std::tm stime;
	gmtime_s(&stime, &time);
	DateTime tm(stime.tm_year + 1900, stime.tm_mon + 1, stime.tm_mday, stime.tm_hour, stime.tm_min, stime.tm_sec);
	Eci eci(sgp4.FindPosition(tm));
	CoordGeodetic geo(eci.ToGeodetic());
	auto longitude(geo.longitude);
	auto latitude(geo.latitude);
	longitude += MathConstants::PI;
	latitude -= MathConstants::PI / 2;
	latitude = -latitude;
	return { longitude, latitude };
}

void Satellite::updatePosition(std::time_t time) {
	if (!_shown) return;
	if (time == 0) calculate();
	else {
		std::tm stime;
		gmtime_s(&stime, &time);
		calculate(stime);
	}
	satpos.longitude += MathConstants::PI;
	satpos.latitude -= MathConstants::PI / 2;
	satpos.latitude = -satpos.latitude;
}

void Satellite::render() {
	if (!_shown) return;
	renderTrajectory();
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto satSize(texture->getDimensions());

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

Satellite::~Satellite() {}
