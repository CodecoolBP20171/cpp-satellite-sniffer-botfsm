#pragma once
#include <cmath>

namespace satelliteSniffer {
	// Earth constatnts
	const double PI = cos(-1);
	const double EarthRadius = 6378.16;
	const double MinutesPerDay = 24 * 60.0;
	const double SecondsPerDay = MinutesPerDay * 60;
	const double RadiansPerDegree = PI / 180;

	// Celestial mechanich constants
	const double GM = 398600;			// Gravitational constants in km^3 / s^2
	const double EarthFlat = 1 / 298.25;	// Earth Flattening Coefficiens
	const double SiderealSolar = 1.0027379093;
	const double SidRate = PI * SiderealSolar / SecondsPerDay;
	
	const double Epsilon = RadiansPerDegree / 36000;  // 1 arc. sec.

	// Calculate date to Julian date
	double calcMJD(int mn, double dy, int yr);
}