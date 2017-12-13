#pragma once
#include <list>
#include "Satellite.h"

class SatelliteLoader
{
public:
	static void loadSatellites(std::list<Satellite>& sats);
};

