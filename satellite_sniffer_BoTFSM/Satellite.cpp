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
	satpos = eci.ToGeodetic();
}

void Satellite::calculate() {
	DateTime tm(DateTime::Now(true));
	Eci eci(sgp4.FindPosition(tm));
	satpos = eci.ToGeodetic();
}


CoordGeodetic Satellite::getPositionAtTime(std::time_t& time) {
	std::tm stime;
	gmtime_s(&stime, &time);
	DateTime tm(stime.tm_year + 1900, stime.tm_mon + 1, stime.tm_mday, stime.tm_hour, stime.tm_min, stime.tm_sec);
	Eci eci(sgp4.FindPosition(tm));
	CoordGeodetic geo(eci.ToGeodetic());
	geo.longitude += MathConstants::PI;
	geo.latitude -= MathConstants::PI / 2;
	geo.latitude = -geo.latitude;
	return geo;
}

CoordGeodetic & Satellite::getPosition()
{
	return satpos;
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


Satellite::~Satellite() {}

void Satellite::toggleShown()
{
	_shown = !_shown;
}

int Satellite::getDelta(std::time_t& time)
{
	OrbitalElements oe(tle);
	auto pos(getPositionAtTime(time));
	auto rate(pos.altitude / oe.RecoveredSemiMajorAxis());
	return static_cast<int>(round(rate / 24));
}

bool & Satellite::isShown()
{
	return _shown;
}

std::string & Satellite::getName()
{
	return name;
}

std::string & Satellite::getType()
{
	return type;
}
