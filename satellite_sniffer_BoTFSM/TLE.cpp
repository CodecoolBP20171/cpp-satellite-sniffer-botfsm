#include "stdafx.h"
#include "TLE.h"
#include "CelMechConst.h"
#include <cmath>
#include <sstream>

TLE::TLE(std::string line1, std::string line2) {
	std::stringstream l1s(line1);
	std::stringstream l2s(line2);
	std::string current;
	l1s >> current >> current >> current;
	l1s >> EpochTime;
	l2s >> current >> current;
	l2s >> Inclination >> RAAN >> current >> ArgPerigree >> MeanAnomaly >> MeanMotion;
	Eccentricity = std::stod("." + current);
}

std::pair<double, double> TLE::calculate(double t) {
	double longi(0), lati(0);

	double satX, satY, satZ;
	double radius;
	double semiMajorAxis(calcSemiMajorAxis());
	double trueAnomaly(calcTrueAnomaly());

	double RAANPrecession, PerigreePrecession;
	getPrecession(semiMajorAxis, RAANPrecession, PerigreePrecession);
	getSatPosition(semiMajorAxis, RAANPrecession, PerigreePrecession, t, trueAnomaly, satX, satY, satZ, radius);

	double r;
	double SidDay = floor(EpochTime - 0.5);
	double T = floor(EpochTime) / 36525;
	double T2 = T * T;
	double SidReference = (6.6460656 + 2400.051262*T + 0.00002581*T2) / 24;
	SidReference -= floor(SidReference);

	r = sqrt(pow(satX, 2) + pow(satY, 2) + pow(satZ, 2));

	longi = satelliteSniffer::PI * 2 * ((T - SidDay) * satelliteSniffer::SiderealSolar + SidReference) - atan2(satY, satX);

	rangeNorm(longi, satelliteSniffer::PI * 2);
	if (longi > satelliteSniffer::PI) longi -= satelliteSniffer::PI * 2;

	lati = atan(satZ / sqrt(pow(satX, 2) + pow(satY, 2)));



	return { longi, lati };
}

void TLE::rangeNorm(double & value, double range)
{
	value -= range * floor(value / range);
}

double TLE::calcSemiMajorAxis()
{
	return 331.25 * exp(2 * log(satelliteSniffer::MinutesPerDay / MeanMotion) / 3);
}

double TLE::calcTrueAnomaly()
{
	// kepler ( meananomaly, eccentricity)
	double EccAnomaly(MeanAnomaly);
	double Error;
	double TrueAnomaly;

	do {
		Error = (EccAnomaly - Eccentricity*sin(EccAnomaly) - MeanAnomaly) / (1 - Eccentricity*cos(EccAnomaly));
		EccAnomaly -= Error;
	} while (abs(Error) >= satelliteSniffer::Epsilon);

	if( abs(EccAnomaly - satelliteSniffer::PI) ){
		TrueAnomaly = satelliteSniffer::PI;
	}
	else
	{
		TrueAnomaly = 2 * atan(sqrt((1 + Eccentricity) / (1 - Eccentricity)) * tan(EccAnomaly / 2));
	}
	if (TrueAnomaly < 0) TrueAnomaly += satelliteSniffer::PI * 2;
	return TrueAnomaly;
}

void TLE::getPrecession(double SemiMajorAxis, double & RAANPrecession, double & PerigeePrecession)
{
	RAANPrecession = 9.95 * pow(satelliteSniffer::EarthRadius / SemiMajorAxis, 3.5) * cos(Inclination) / pow(1 - pow(Eccentricity, 2), 2) * satelliteSniffer::RadiansPerDegree;

	PerigeePrecession = 4.97 * pow(satelliteSniffer::EarthRadius / SemiMajorAxis, 3.5) * (5 * pow(cos(Inclination),2) -1) / pow(1 - pow(Eccentricity, 2), 2) * satelliteSniffer::RadiansPerDegree;
}

void TLE::getSatPosition(double SemiMajorAxis, double RAANPrecession, double PerigeePrecession, double currTime, double TrueAnomaly, double & x, double & y, double & z, double & radius)
{
	double Raan, ArgPerig;

	double Xw, Yw;
	double tmp;
	double Px, Qx, Py, Qy, Pz, Qz;
	double CosArgPerigee, SinArgPerigee;
	double CosRAAN, SinRAAN, CoSinclination, SinInclination;

	radius = SemiMajorAxis * (1 - pow(Eccentricity, 2)) / (1 + Eccentricity * cos(TrueAnomaly));
	if (radius <= satelliteSniffer::EarthRadius) radius = satelliteSniffer::EarthRadius;

	Xw = radius * cos(TrueAnomaly);
	Yw = radius * sin(TrueAnomaly);

	tmp = sqrt(satelliteSniffer::GM / (SemiMajorAxis * (1 - pow(Eccentricity, 2))));

	ArgPerig = ArgPerigree + (currTime - EpochTime) * PerigeePrecession;

	Raan = RAAN - (currTime - EpochTime) * RAANPrecession;

	CosRAAN = cos(RAAN);
	SinRAAN = sin(RAAN);
	CosArgPerigee = cos(ArgPerig);
	SinArgPerigee = sin(ArgPerig);
	CoSinclination = cos(Inclination);
	SinInclination = sin(Inclination);

	Px = CosArgPerigee*CosRAAN - SinArgPerigee*SinRAAN*CoSinclination;
	Py = CosArgPerigee*SinRAAN + SinArgPerigee*CosRAAN*CoSinclination;
	Pz = SinArgPerigee*SinInclination;
	Qx = -SinArgPerigee*CosRAAN - CosArgPerigee*SinRAAN*CoSinclination;
	Qy = -SinArgPerigee*SinRAAN + CosArgPerigee*CosRAAN*CoSinclination;
	Qz = CosArgPerigee*SinInclination;

	x = Px * Xw + Qx * Yw;
	y = Py * Xw + Qy * Yw;
	z = Pz * Xw + Qz * Yw;
}

TLE::~TLE() {}
