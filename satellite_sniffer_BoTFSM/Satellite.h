#pragma once
#include <utility>
#include <list>
#include "TLE.h"
class Satellite {
public:
	Satellite(std::string name, std::string noradId);
	std::pair<double, double> calculate(double time = 0);
	~Satellite();
private:
	std::string name, noradId;
	std::list<TLE> tleData;
};

