// satellite_sniffer_BoTFSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <iostream>

#include "Satellite.h"

int main(int argc, char* argv[])
{
	std::list<Satellite> sats;
	std::ifstream file("../satellites/satellites.dat");
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream sat(line);
		std::string name, noradId;
		std::getline(sat, name, ';');
		std::getline(sat, noradId, ';');
		sats.emplace_back(name, noradId);
	}
	file.close();
	auto pos(sats.begin()->calculate());
	std::cout << pos.first  << " " << pos.second << std::endl;
    return 0;
}

