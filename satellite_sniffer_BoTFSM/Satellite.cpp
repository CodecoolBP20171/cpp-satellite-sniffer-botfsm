#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "Satellite.h"
#include "Resources.h"
#include "MechStandards.h"


Satellite::Satellite(std::string name, std::string noradId)
	: name(name),
	noradId(noradId),
	texture(Resources::getInstance()->getSat(name)) {
	std::ifstream file("satellites/" + noradId + ".dat");
	std::string line1, line2;
	while (std::getline(file, tle1)) {
		std::getline(file, tle2);
	}
	file.close();
}

// TODO make as non parametered
std::pair<double, double> Satellite::calculate(std::tm& time) {
	// '1984/05/30 16:23:45.12'
	std::stringstream strtime;
	strtime << time.tm_year + 1900 << "/" << time.tm_mon + 1 << "/" << time.tm_mday << " " 
			<< time.tm_hour << ":" << time.tm_min << ":" << time.tm_sec;
	std::string cmd("python getSatPos.py \"" + name + "\" \"" + tle1 + "\" \"" + tle2 + "\" \"" + strtime.str() + "\"");
	std::string result(exec(cmd.c_str()));

	std::stringstream stris(result);

	double longi, lati;
	stris >> longi >> lati;
	return { longi, lati };
}

std::pair<double, double> Satellite::calculate() {

	auto utime(time(0));
	std::tm time;
	gmtime_s(&time, &utime);
	return calculate(time);
}

void Satellite::render(SDL_Rect & mapSize, std::time_t time) {
	std::pair<double, double> satpos;
	if (time == 0) satpos = calculate();
	else {
		std::tm stime;
		gmtime_s(&stime, &time);
		satpos = calculate(stime);
	}
	std::cout << satpos.first << " " << satpos.second << std::endl;
	satpos.first += satelliteSniffer::PI;
	satpos.second -= satelliteSniffer::PI / 2;
	satpos.second = -satpos.second;

	auto satSize(texture->getDimensions());
	SDL_Rect satRect = { 
		static_cast<int>(round(satpos.first / (satelliteSniffer::PI * 2) * mapSize.w - satSize.w/2)),
		static_cast<int>(round(satpos.second / (satelliteSniffer::PI) * mapSize.h - satSize.h/2)),
		satSize.w,
		satSize.h 
	};
	std::cout << satRect.x << " " << satRect.y << std::endl;
	texture->render(&satRect);
}

Satellite::~Satellite() {}

std::string Satellite::exec(const char* cmd)
{
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}
