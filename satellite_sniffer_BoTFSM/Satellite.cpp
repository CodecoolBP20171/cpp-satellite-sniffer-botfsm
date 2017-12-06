#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <time.h>

#include "Satellite.h"
#include "MechStandards.h"


Satellite::Satellite(std::string name, std::string noradId)
	: name(name),
	noradId(noradId) {
	std::ifstream file("../satellites/" + noradId + ".dat");
	std::string line1, line2;
	while (std::getline(file, line1)) {
		std::getline(file, line2);
		tleData.emplace_back(line1, line2);
	}
	file.close();
}

std::pair<double, double> Satellite::calculate(std::tm& time) {
	double day(time.tm_yday + (time.tm_hour * 60.0 * 60 + time.tm_min * 60.0 + time.tm_sec) / (24.0 * 60 * 60));
	return tleData.begin()->calculate(satelliteSniffer::calcMJD(1, day, 1900 + time.tm_year));
}

std::pair<double, double> Satellite::calculate() {
	auto utime(time(0));
	std::tm time;
	gmtime_s(&time, &utime);
	return calculate(time);
}

Satellite::~Satellite() {}
