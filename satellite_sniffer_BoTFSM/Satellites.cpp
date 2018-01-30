#include "stdafx.h"
#include "Satellites.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <string>

std::shared_ptr<Satellites> Satellites::instance = nullptr;

Satellites::Satellites()
{
	loadSatellites();
	for (auto& sat : sats) {
		UISats.emplace_back(sat);
	}
}


std::shared_ptr<Satellites>& Satellites::getInstance()
{
	if (!instance) {
		instance = std::shared_ptr<Satellites>(new Satellites());
	}
	return instance;
}

void Satellites::updatePosition()
{
	for (auto& sat : sats) {
		sat.updatePosition();
	}
}

Satellites::~Satellites()
{
}

std::list<UISatellite>& Satellites::getUISatellites()
{
	return UISats;
}

void Satellites::renderUISatellits()
{
	for (auto& sat : UISats) {
		sat.render();
	}
}

void Satellites::renderPopupSatellits()
{
	for (auto& sat : UISats) {
		sat.popupRender();
	}
}

void Satellites::loadSatellites()
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

void Satellites::saveSatelliteList()
{
	std::ofstream file(Config::getStringOption("DataFiles", "DATA_DIR") + "/" + Config::getStringOption("DataFiles", "LIST_FILE"), std::ios_base::trunc);
	for (auto& sat : sats) {
		file << sat.toString();
	}
	file.close();
}