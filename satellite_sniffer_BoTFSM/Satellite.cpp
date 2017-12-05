#include "stdafx.h"

#include <fstream>
#include <sstream>

#include "Satellite.h"


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

std::pair<double, double> Satellite::calculate(double time) {
	if (time == 0) {
		time = tleData.begin()->EpochTime;
		// TODO set time to now() (need to convert to double format)
	}
	return tleData.begin()->calculate(time);
}

Satellite::~Satellite() {}
