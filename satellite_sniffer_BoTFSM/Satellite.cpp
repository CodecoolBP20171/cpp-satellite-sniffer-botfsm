#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "Satellite.h"
#include "Resources.h"
#include "MechStandards.h"
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
	text(new ScreenText(name)) {
	setPosition();
	for (int i = 0; i < 8; ++i) {
		pointsForward.emplace_back(Resources::getInstance()->getPoint());
		pointsBackward.emplace_back(Resources::getInstance()->getPoint());
	}
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
	GeoCoordinate currentSatPos(satpos);
	std::time_t now;
	std::time(&now);
	auto delta((24 * 60 * 60) / tle.MeanMotion() / 200);

	for (auto point : pointsForward) {
		now += delta;
		renderPoint(now, point);
	}
	now = 0;
	std::time(&now);
	for (auto point : pointsBackward) {
		now -= delta;
		renderPoint(now, point);
	}
	satpos = currentSatPos;
}

void Satellite::renderPoint(std::time_t & now, std::shared_ptr<Texture>& point) {
	auto mapSize(Resources::getInstance()->getMapDimensions());
	setPosition(now);
	auto pointSize(point->getDimensions());

	SDL_Rect satRect = {
		static_cast<int>(round(satpos.longitude / (satelliteSniffer::PI * 2) * mapSize.w - pointSize.w / 2)),
		static_cast<int>(round(satpos.latitude / (satelliteSniffer::PI) * mapSize.h - pointSize.h / 2)),
		pointSize.w,
		pointSize.h
	};
	point->render(&satRect);
}

void Satellite::setPosition(std::time_t time) {
	if (time == 0) calculate();
	else {
		std::tm stime;
		gmtime_s(&stime, &time);
		calculate(stime);
	}
	satpos.longitude += satelliteSniffer::PI;
	satpos.latitude -= satelliteSniffer::PI / 2;
	satpos.latitude = -satpos.latitude;
}

void Satellite::render() {
	renderTrajectory();
	auto mapSize(Resources::getInstance()->getMapDimensions());
	auto satSize(texture->getDimensions());

	SDL_Rect satRect = {
		static_cast<int>(round(satpos.longitude / (satelliteSniffer::PI * 2) * mapSize.w - satSize.w / 2)),
		static_cast<int>(round(satpos.latitude / (satelliteSniffer::PI) * mapSize.h - satSize.h / 2)),
		satSize.w,
		satSize.h
	};
	texture->render(&satRect);
	satRect.x + satSize.w;
	auto textPos(text->getDimensions());
	textPos.x = satRect.x + satSize.w;
	textPos.y = satRect.y;
	text->render(&textPos);
}

Satellite::~Satellite() {}
