#include "stdafx.h"
#include "SatelliteLoader.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <string>

void SatelliteLoader::loadSatellites(std::list<Satellite>& sats)
{
	std::ifstream file(Config::getStringOption("DataFiles", "DATA_DIR") + "/" + Config::getStringOption("DataFiles", "LIST_FILE"));
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream sat(line);
		std::string name, noradId, type;
		int visible;
		std::getline(sat, name, ';');
		std::getline(sat, noradId, ';');
		std::getline(sat, type, ';');
		sat >> visible;
		sats.emplace_back(name, noradId, type, visible);
	}
	file.close();
}

void SatelliteLoader::saveSatelliteList(std::list<UISatellite>& sats)
{
	std::ofstream file(Config::getStringOption("DataFiles", "DATA_DIR") + "/" + Config::getStringOption("DataFiles", "LIST_FILE"), std::ios_base::trunc);
	for (auto& sat : sats) {
		file << sat.toString();
	}
	file.close();
}
