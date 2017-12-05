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
	double MeanMotion;

	void rangeNorm(double& value, double range);
	double calcSemiMajorAxis();
	double calcTrueAnomaly();

	void getPrecession(double SemiMajorAxis, double& RAANPrecession, double& PerigeePrecession);
	void getSatPosition(double SemiMajorAxis, double RAANPrecession, double PerigeePrecession, double currTime, double TrueAnomaly, double& x, double& y, double& z, double& radius);
};

