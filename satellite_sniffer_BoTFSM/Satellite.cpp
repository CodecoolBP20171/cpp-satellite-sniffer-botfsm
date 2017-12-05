#include "stdafx.h"
#include "Satellite.h"


Satellite::Satellite() {}

void Satellite::addTLE(std::string line1, std::string line2) {
	tleData.emplace_back(line1, line2);
}

std::pair<double, double> Satellite::calculate(double time) {
	if (time == 0) {
		// TODO set time to now() (need to convert to double format)
	}
	return tleData.begin()->calculate(time);
}

Satellite::~Satellite() {}
