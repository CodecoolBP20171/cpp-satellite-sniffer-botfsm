#pragma once
#include <list>
#include "Satellite.h"

class LoadSatellites
{
public:
	static void load(std::list<Satellite>& sats);
private:
	LoadSatellites();
	~LoadSatellites();
};

