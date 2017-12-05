#pragma once
#include <utility>
#include <list>
#include "TLE.h"
class Satellite {
public:
	Satellite();
	void addTLE(std::string line1, std::string line2);
	std::pair<double, double> calculate(double time = 0);
	~Satellite();
private:
	std::list<TLE> tleData;
};

