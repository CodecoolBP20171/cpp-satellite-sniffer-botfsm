#include "stdafx.h"
#include "SatelliteLoader.h"
#include <fstream>
#include <sstream>
#include <string>

void SatelliteLoader::loadSatellites(std::list<Satellite>& sats)
{
	std::ifstream file("satellites/satellites.dat");
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream sat(line);
		std::string name, noradId, type;
		std::getline(sat, name, ';');
		std::getline(sat, noradId, ';');
		std::getline(sat, type, ';');
		sats.emplace_back(name, noradId, type);
	}
	file.close();
}
