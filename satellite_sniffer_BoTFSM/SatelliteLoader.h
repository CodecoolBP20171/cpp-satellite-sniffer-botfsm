#pragma once
#include <list>
#include "Satellite.h"
#include "UISatellite.h"

class SatelliteLoader
{
public:
	static void loadSatellites(std::list<Satellite>& sats);
	static void saveSatelliteList(std::list<UISatellite>& sats);
};

