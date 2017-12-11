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

#include <SGP4.h>
#include <CoordGeodetic.h>


Satellite::Satellite(std::string name, std::string noradId, std::string type)
	: name(name),
	noradId(noradId),
	type(type),
	texture(Resources::getInstance()->getSat(type)) {
	std::ifstream file("satellites/" + noradId + ".dat");
	std::getline(file, tle1);
	std::getline(file, tle2);
	file.close();
}


std::pair<double, double> Satellite::calculate(std::tm& time) {
	Tle tle(name, tle1, tle2);
	SGP4 sgp4(tle);
	DateTime tm(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	Eci eci(sgp4.FindPosition(tm));
	CoordGeodetic geo(eci.ToGeodetic());
	return { geo.longitude, geo.latitude };
}

std::pair<double, double> Satellite::calculate() {
	Tle tle(name, tle1, tle2);
	SGP4 sgp4(tle);
	DateTime tm(DateTime::Now(true));
	Eci eci(sgp4.FindPosition(tm));
	CoordGeodetic geo(eci.ToGeodetic());
	return { geo.longitude, geo.latitude };
}

void Satellite::render(SDL_Rect & mapSize, std::time_t time) {
	if (tle1 == "" || tle2 == "") return;
	std::pair<double, double> satpos;
	if (time == 0) satpos = calculate();
	else {
		std::tm stime;
		gmtime_s(&stime, &time);
		satpos = calculate(stime);
	}
	satpos.first += satelliteSniffer::PI;
	satpos.second -= satelliteSniffer::PI / 2;
	satpos.second = -satpos.second;

	auto satSize(texture->getDimensions());
	SDL_Rect satRect = {
		static_cast<int>(round(satpos.first / (satelliteSniffer::PI * 2) * mapSize.w - satSize.w / 2)),
		static_cast<int>(round(satpos.second / (satelliteSniffer::PI) * mapSize.h - satSize.h / 2)),
		satSize.w,
		satSize.h
	};
	texture->render(&satRect);
	satRect.x += satSize.w;
	Resources::getInstance()->renderText(name, satRect);
}

Satellite::~Satellite() {}

std::string Satellite::exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}
