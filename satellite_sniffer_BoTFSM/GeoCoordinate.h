#pragma once
struct GeoCoordinate
{
	GeoCoordinate(double longi = 0, double lati = 0);

	double longitude;
	double latitude;

	~GeoCoordinate();
};

