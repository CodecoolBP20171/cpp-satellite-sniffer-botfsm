#pragma once
#include <string>
class TLE {
public:
	TLE(std::string line1, std::string line2);
	~TLE();
	std::pair<double, double> calculate(double t);
private:
	double EpochTime;
	double RAAN;
	double ArgPerigree;
	double Inclination;
	double Eccentricity;
	double MeanAnomaly;
};

